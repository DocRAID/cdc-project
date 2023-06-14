cmake_minimum_required(VERSION 3.19)
project(cdc_run)

find_package(RdKafka CONFIG REQUIRED)
find_package(Poco REQUIRED [COMPONENTS PocoFoundation PocoNet PocoJSON])

set(CMAKE_CXX_STANDARD 17)

add_executable(cdc_run function.cpp main.cpp)

target_link_libraries(cdc_run RdKafka::rdkafka RdKafka::rdkafka++ PocoFoundation PocoNet PocoJSON)
