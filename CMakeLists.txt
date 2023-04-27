cmake_minimum_required(VERSION 3.19)
project(poco_test)

set(CMAKE_CXX_STANDARD 14)

add_executable(poco_test main.cpp)

target_link_libraries(poco_test PocoFoundation PocoNet PocoJSON)
