cmake_minimum_required(VERSION 2.6)
project(multithreadcpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")

add_executable(HandleThreadExceptions HandleThreadExceptions.cpp)
target_link_libraries(HandleThreadExceptions
    pthread
)

