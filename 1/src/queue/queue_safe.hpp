#ifndef __QUEUE_SAFE_HPP__
#define __QUEUE_SAFE_HPP__

#include <memory>
#include <mutex>
#include <thread>
#include <condition_variable>
// 6.7
template<typename T>
class threadsafe_queue {
private:
	struct node {
		std::shared_ptr<T> data;
		std::unique_ptr<node> next;
	};

	std::mutex head_mutex;
	std::unique_ptr<node> head;
	std::mutex tail_mutex;
	node* tail;
	std::condition_variable data_cond;
public:
	threadsafe_queue() :
			head(new node), tail(head.get()) {
	}
	threadsafe_queue(const threadsafe_queue& other) = delete;
	threadsafe_queue& operator=(const threadsafe_queue& other) = delete;
	void push(T new_value);

private:
	std::unique_ptr<node> try_pop_head() {
		std::lock_guard < std::mutex > head_lock(head_mutex);
		if (head.get() == get_tail()) {
			return std::unique_ptr<node>();
		}
		return pop_head();
	}
	std::unique_ptr<node> try_pop_head(T& value) {
		std::lock_guard < std::mutex > head_lock(head_mutex);
		if (head.get() == get_tail()) {
			return std::unique_ptr<node>();
		}
		value = std::move(*head->data);
		return pop_head();
	}
	node* get_tail() {
		std::lock_guard < std::mutex > tail_lock(tail_mutex);
		return tail;
	}
	std::unique_ptr<node> pop_head() {
		std::unique_ptr < node > old_head = std::move(head);
		head = std::move(old_head->next);
		return old_head;
	}
	std::unique_lock<std::mutex> wait_for_data()

	{
		std::unique_lock < std::mutex > head_lock(head_mutex);
		data_cond.wait(head_lock, [&] {return head.get()!=get_tail();});
		return std::move(head_lock);

	}
	std::unique_ptr<node> wait_pop_head() {
		std::unique_lock < std::mutex > head_lock(wait_for_data());
		return pop_head();
	}
	std::unique_ptr<node> wait_pop_head(T& value) {
		std::unique_lock < std::mutex > head_lock(wait_for_data());
		value = std::move(*head->data);
		return pop_head();
	}
public:
	std::shared_ptr<T> try_pop() {
		std::unique_ptr < node > old_head = try_pop_head();
		return old_head ? old_head->data : std::shared_ptr<T>();
	}
	bool try_pop(T& value) {
		bool res = false;
		std::unique_ptr<node> const old_head = try_pop_head(value);
		if (old_head)
		{
			res = true;
			value = std::move(*head->data);
		}
		return res;
	}
	void empty() {
		std::lock_guard < std::mutex > head_lock(head_mutex);
		return (head.get() == get_tail());
	}
	std::shared_ptr<T> wait_and_pop() {
		std::unique_ptr<node> const old_head = wait_pop_head();
		return old_head->data;
	}
	void wait_and_pop(T& value) {
		std::unique_ptr<node> const old_head = wait_pop_head(value);
	}
};

#endif
