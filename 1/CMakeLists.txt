cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME williams_01)

set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/output_data)

file(GLOB HOST_SRC "./src/*.cpp" "./src/*.c" "./src/queue/*.cpp" "./src/queue/*.hpp")

message("CMAKE_CURRENT_BINARY_DIR - ${CMAKE_CURRENT_BINARY_DIR}")
message("CMAKE_CURRENT_SOURCES_DIR - ${CMAKE_CURRENT_SOURCE_DIR}")
message("Input image file - ${DATA_SRC}")
message("Boost library dir - ${Boost_LIBRARY_DIRS}")
message("Boost include dir - ${Boost_INCLUDE_DIRS}")
message("Boost libraries - ${Boost_LIBRARIES}")
message(" - ${Boost_LIBRARIES}")

add_definitions(-std=c++17)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src" ${Boost_INCLUDE_DIRS})

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()



link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../bin")
add_executable(${PROJECT_NAME} ${HOST_SRC})

target_link_libraries(${PROJECT_NAME} stdc++fs mysqlcppconn-static)

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND "${CMAKE_COMMAND}" -E copy
#    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}"
#    "${CMAKE_CURRENT_SOURCE_DIR}/../bin/${PROJECT_NAME}"
#    COMMENT "Copying to output dir")
