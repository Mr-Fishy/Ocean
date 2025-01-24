#include <Ocean/Ocean.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Factorial of 0 is 1", "[Numerics]") {
    
    REQUIRE( factorial(0) == 1 );

}

TEST_CASE("Factorials Are Computed", "[Numerics]") {
    
    REQUIRE( factorial(1) == 1 );
    REQUIRE( factorial(2) == 2 );
    REQUIRE( factorial(3) == 6 );
    REQUIRE( factorial(10) == 3628800 );

}
