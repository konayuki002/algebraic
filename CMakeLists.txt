cmake_minimum_required(VERSION 3.10)

project(algebraic CXX)

set(CMAKE_CXX_FLAGS "-O2 -std=c++1z -Wall")

enable_testing()

add_subdirectory(lib)
add_subdirectory(test)
