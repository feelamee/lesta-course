#include "catch2/catch_test_macros.hpp"
#include <catch2/catch_all.hpp>

#include <nano/utils.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>

std::string
random_string(std::size_t size)
{
    auto randchar = []() -> char
    {
        const char charset[] =
            "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index];
    };
    std::string str(size, 0);
    std::generate_n(str.begin(), size, randchar);
    return str;
}

TEST_CASE("extract file")
{
    namespace fs = std::filesystem;
    std::error_code ec;
    fs::path tmpfile = fs::temp_directory_path(ec) / "extract-file-test";
    REQUIRE_FALSE(ec.default_error_condition());
    ec.clear();

    std::string output = random_string(500);
    {
        std::ofstream dst(tmpfile);
        REQUIRE(dst);
        dst << output;
    }

    std::string input;
    REQUIRE_FALSE(nano::extract_file(input, tmpfile));
    REQUIRE(input == output);

    fs::remove(tmpfile, ec);
    REQUIRE_FALSE(ec.default_error_condition());
}
