cmake_minimum_required(VERSION 3.6)
project(ESP)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lm")

set(SOURCE_FILES
    hw6/assa.c)

add_executable(ESP ${SOURCE_FILES})