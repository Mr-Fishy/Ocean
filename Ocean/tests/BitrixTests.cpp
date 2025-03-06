#include <Ocean/Ocean.hpp>

#include "base/Tests.hpp"

// std
#include <iostream>
#include <sstream>

TEST_CASE(Bitrix2D_Default_Constructor) {
    Bitrix2D matrix;

    std::cerr << "Default Constructor" << std::endl;

    REQUIRE(matrix.Empty());
}

TEST_CASE(Bitrix2D_Constructor_with_Width) {
    Bitrix2D matrix(5, false);

    REQUIRE(matrix.Get(0, 0) == false); // Default value should be false
    REQUIRE(matrix.Get(4, 0) == false); // Default value should be false
}

TEST_CASE(Bitrix2D_Constructor_with_Width_and_Height) {
    Bitrix2D matrix(5, 3);

    REQUIRE(matrix.Get(0, 0) == false); // Default value should be false
    REQUIRE(matrix.Get(4, 2) == false); // Default value should be false
}

TEST_CASE(Bitrix2D_Set_and_Get_Methods) {
    Bitrix2D matrix(5, 5);
    matrix.Set(2, 2, true);
    REQUIRE(matrix.Get(2, 2) == true);
    matrix.Set(2, 2, false);
    REQUIRE(matrix.Get(2, 2) == false);
}

TEST_CASE(Bitrix2D_Reserve_Method) {
    Bitrix2D matrix(2, 2);

    matrix.Set(0, 0, true);
    matrix.Reserve(4, false); // Increase width to 4, default value false

    REQUIRE(matrix.Get(0, 0) == true); // Should still be true
    REQUIRE(matrix.Get(3, 0) == false); // New position should be false
}

TEST_CASE(Bitrix2D_Reserve_with_Height) {
    Bitrix2D matrix(2, 2);

    matrix.Set(1, 1, true);
    matrix.Reserve(2, 4, false); // Increase height to 4, default value false

    REQUIRE(matrix.Get(1, 1) == true); // Should still be true
    REQUIRE(matrix.Get(1, 3) == false); // New position should be false
}

TEST_CASE(Bitrix2D_Output_Stream_Operator) {
    Bitrix2D matrix(3, 3);

    matrix.Set(0, 0, true);
    matrix.Set(1, 1, true);
    matrix.Set(2, 2, true);

    std::ostringstream oss;
    oss << matrix;
    REQUIRE(oss.str() == "");
}

// MAIN { RUN_TESTS(); }
