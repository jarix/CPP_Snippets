cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

add_definitions(-std=c++11)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(cpp_snippets)

add_executable(socket_server src/socket_server.cpp)
add_executable(socket_client src/socket_client.cpp)
