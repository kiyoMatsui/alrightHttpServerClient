// catch2 file to stop long compiles
#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

TEST_CASE("1: All test cases reside in other .cpp files (empty)",
          "[multi-file:1]") {}

