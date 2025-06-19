add_executable(tests test_array_utils.cpp ../src/array_utils.cpp)
target_link_libraries(tests doctest)