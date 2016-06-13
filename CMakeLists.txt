cmake_minimum_required(VERSION 3.2)
project(thingy)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")

set(SOURCE_FILES
    ramtest.c)

add_executable(thingy ${SOURCE_FILES})