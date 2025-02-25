#pragma once

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

    SECTION("Zero Case") {

        INFO("Expect 1 From \'0!\'");

        REQUIRE( oFactorial(0) == 1 );
    
    }

    SECTION("Primary Tests") {

        REQUIRE( oFactorial(1) == 1 );
        REQUIRE( oFactorial(2) == 2 );
        REQUIRE( oFactorial(3) == 6 );
        REQUIRE( oFactorial(10) == 3628800 );
        REQUIRE( oFactorial(20) == 2432902008176640000 );

    }
}
