#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "/Users/vvv/Documents/programming/OS/lab4/doctest/doctest/doctest.h"
#include <fstream>
#include <string>

TEST_CASE("File creation test") {
    std::string test_file = "test.bin";
    {
        std::ofstream file(test_file);
        REQUIRE(file.is_open());
    }
    REQUIRE(std::ifstream(test_file).good());
    std::remove(test_file.c_str());
}

TEST_CASE("Message length validation") {
    const int MAX_LEN = 20;
    std::string long_msg(MAX_LEN + 10, 'a');
    std::string valid_msg(MAX_LEN, 'b');
    
    REQUIRE(long_msg.length() > MAX_LEN);
    REQUIRE(valid_msg.length() == MAX_LEN);
}