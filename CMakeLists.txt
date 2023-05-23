cmake_minimum_required(VERSION 3.19)
project(cdc_run)

set(CMAKE_CXX_STANDARD 14)

add_executable(cdc_run function.cpp main.cpp)

target_link_libraries(cdc_run PocoFoundation PocoNet PocoJSON)
