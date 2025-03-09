#include <Ocean/Ocean.hpp>

#include "./Base/Tests.hpp"

// std
#include <sstream>

TEST_CASE(Bix8_Default_Constructor) {
    Bix8 bix;

    REQUIRE(bix.Raw() == 0);
}

TEST_CASE(Bix8_Constructor_with_Value) {
    Bix8 bix(0b10101010);

    REQUIRE(bix.Raw() == 0b10101010);
}

TEST_CASE(Bix8_Constructor_with_Initializer_List) {
    Bix8 bix = { true, false, true, false, true, false, false, false };

    REQUIRE(bix.Raw() == 0b10101000);
}

TEST_CASE(Bix8_Equality_Operator) {
    Bix8 bix1(0b11110000);
    Bix8 bix2(0b11110000);
    Bix8 bix3(0b00001111);
    
    REQUIRE(bix1 == bix2);
    REQUIRE(bix1 != bix3);
}

TEST_CASE(Bix8_Index_Operator) {
    Bix8 bix(0b10101000);
    
    REQUIRE(bix[0] == true);
    REQUIRE(bix[1] == false);
    REQUIRE(bix[2] == true);
    REQUIRE(bix[3] == false);
    REQUIRE(bix[4] == true);
    REQUIRE(bix[5] == false);
    REQUIRE(bix[6] == false);
    REQUIRE(bix[7] == false);
}

TEST_CASE(Bix8_At_Method) {
    Bix8 bix(0b10101000);

    CONSOLE_OUT << bix << std::endl;

    REQUIRE(bix.At(0) == true);
    REQUIRE(bix.At(1) == false);
    REQUIRE(bix.At(2) == true);
    REQUIRE(bix.At(3) == false);
    REQUIRE(bix.At(4) == true);
    REQUIRE(bix.At(5) == false);
    REQUIRE(bix.At(6) == false);
    REQUIRE(bix.At(7) == false);
}

TEST_CASE(Bix8_Flip_Method) {
    Bix8 bix(0b10101010);

    bix.Flip(0);
    REQUIRE(bix.Raw() == 0b00101010);

    bix.Flip(1);
    REQUIRE(bix.Raw() == 0b01101010);
}

TEST_CASE(Bix8_Set_Method) {
    Bix8 bix(0b00000000);

    bix.Set(0, true);
    REQUIRE(bix.Raw() == 0b10000000);

    bix.Set(1, true);
    REQUIRE(bix.Raw() == 0b11000000);

    bix.Set(0, false);
    REQUIRE(bix.Raw() == 0b01000000);
}

TEST_CASE(Bix8_Mask_Method) {
    Bix8 bix(0b11111111);

    bix.Mask(0b00001111);
    REQUIRE(bix.Raw() == 0b00001111);
}

TEST_CASE(Bix8_Clear_Method) {
    Bix8 bix(0b10101010);

    bix.Clear();

    REQUIRE(bix.Raw() == 0);
}

TEST_CASE(Bix8_Output_Stream_Operator) {
    Bix8 bix(0b10101000);

    std::ostringstream oss;
    oss << bix;

    REQUIRE(oss.str() == "10101000");
}
