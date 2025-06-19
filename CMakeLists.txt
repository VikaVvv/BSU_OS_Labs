cmake_minimum_required(VERSION 3.14)
project(Lab2)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory( doctest)
include_directories( doctest)
add_executable(lab2 src/main.cpp src/array_utils.cpp)
add_subdirectory(tests)