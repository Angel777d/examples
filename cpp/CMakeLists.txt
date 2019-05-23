cmake_minimum_required(VERSION 3.7)
project(cpp)

set(CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# include_directories(/usr/local/lib)

add_executable(
        cpp
        main.cpp
        pool2/pool2.h
        pool2/pool2.cpp
        pool2/pool_types.h
)

