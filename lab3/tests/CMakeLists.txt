add_executable(marker_tests
    test_marker.cpp
)

target_include_directories(marker_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_compile_definitions(marker_tests PRIVATE
    DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
    TESTING_MODE
)

add_test(
    NAME marker_tests
    COMMAND $<TARGET_FILE:marker_tests> --no-version --reporters=console
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)