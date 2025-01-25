/**
 * @file NumericTests.hpp
 * @author Evan F.
 * @brief This file is to organize all Numerics.hpp related tests.
 * @date 01-24-2025
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <Ocean/Ocean.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_assertion_result.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>



TEST_CASE("Factorials", "[Numerics]") {
    
    INFO("Expect 1 From \'0!\'");
    SECTION("Zero Case") {
    
        REQUIRE( factorial(0) == 1 );
    
    }

    INFO("Test Within Useable Bounds");
    SECTION("General Test") {

        REQUIRE( factorial(1) == 1 );
        REQUIRE( factorial(2) == 2 );
        REQUIRE( factorial(3) == 6 );
        REQUIRE( factorial(10) == 3628800 );
        REQUIRE( factorial(20) == 2432902008176640000 );

        BENCHMARK("Factorial of 20") {
            return factorial(20);
        };

    }
}
