cmake_minimum_required(VERSION 3.10)
project(EmployeeManager)

set(CMAKE_CXX_STANDARD 11)

# Add your main executables
add_executable(creator creator.cpp)
add_executable(reporter reporter.cpp)
add_executable(main main.cpp)

# Doctest (header-only)
enable_testing()

# Add your test executable
add_executable(runTests tests.cpp)

# Include Doctest header
target_include_directories(runTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# No need to link against a library for Doctest