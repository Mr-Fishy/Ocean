#include <Ocean/Ocean.hpp>

#include "./Base/Tests.hpp"

// std
#include <sstream>

TEST_CASE(DynamicArray_Default_Constructor) {
    DynamicArray<int> arr;
    REQUIRE(arr.Size() == 0);
    REQUIRE(arr.Capacity() == 0);
}

TEST_CASE(DynamicArray_Initial_Size_Constructor) {
    DynamicArray<int> arr(10);

    REQUIRE(arr.Size() == 0);  // Size should be 0 initially
    REQUIRE(arr.Capacity() == 10);  // Capacity should be 10
}

TEST_CASE(DynamicArray_Copy_Constructor) {
    DynamicArray<int> arr1(5);

    for (int i = 0; i < 5; ++i)
        arr1.EmplaceBack(i);

    DynamicArray<int> arr2(arr1);  // Copy constructor

    REQUIRE(arr2.Size() == 5);
    REQUIRE(arr2.Capacity() == 5);  // Should match the original capacity

    for (int i = 0; i < 5; ++i)
        REQUIRE(arr2[i] == i);  // Elements should be the same
}

TEST_CASE(DynamicArray_Assignment_Operator) {
    DynamicArray<int> arr1(5);

    for (int i = 0; i < 5; ++i)
        arr1.EmplaceBack(i);

    DynamicArray<int> arr2 = arr1;  // Use assignment operator

    REQUIRE(arr2.Size() == 5);
    REQUIRE(arr2.Capacity() == 5);

    for (int i = 0; i < 5; ++i)
        REQUIRE(arr2[i] == i);  // Elements should be the same
}

TEST_CASE(DynamicArray_Equality_Operators) {
    DynamicArray<int> arr1(5);

    for (int i = 0; i < 5; ++i)
        arr1.EmplaceBack(i);

    DynamicArray<int> arr2(arr1);
    REQUIRE(arr1 == arr2);  // Arrays should be equal

    arr2.EmplaceBack(5);
    REQUIRE(arr1 != arr2);  // Arrays should be unequal
}

TEST_CASE(DynamicArray_At_Function_Accessor) {
    DynamicArray<int> arr(5);

    for (int i = 0; i < 5; ++i)
        arr.EmplaceBack(i);

    // Valid access
    REQUIRE(arr.At(0) == 0);
    REQUIRE(arr.At(4) == 4);

    // Invalid access (out-of-range)
    REQUIRE_THROW_AS(arr.At(5), Ocean::Exception);
}

TEST_CASE(DynamicArray_Resize_Function) {
    DynamicArray<int> arr(5);

    for (int i = 0; i < 5; ++i)
        arr.EmplaceBack(i);

    arr.Resize(3);  // Resize down

    REQUIRE(arr.Size() == 3);
    REQUIRE(arr.Capacity() == 3);

    arr.Resize(10);  // Resize up

    REQUIRE(arr.Size() == 3);
    REQUIRE(arr.Capacity() == 10);  // Capacity should increase
}

TEST_CASE(DynamicArray_Clear_Function) {
    DynamicArray<int> arr(5);

    for (int i = 0; i < 5; ++i)
        arr.EmplaceBack(i);

    arr.Clear();  // Clear the array

    REQUIRE(arr.Size() == 0);
    REQUIRE(arr.Capacity() == 5);  // Capacity should remain the same, but size is 0
}

TEST_CASE(DynamicArray_Erase_Function) {
    DynamicArray<int> arr(5);

    for (int i = 0; i < 5; ++i)
        arr.EmplaceBack(i);

    arr.Erase(2);  // Remove element at index 2

    REQUIRE(arr.Size() == 4);
    REQUIRE(arr[2] == 3);  // Elements should shift left
}

TEST_CASE(DynamicArray_Emplace_Function) {
    DynamicArray<int> arr(5);

    for (int i = 0; i < 5; ++i)
        arr.EmplaceBack(i);

    arr.Emplace(2, 99);  // Insert 99 at position 2

    REQUIRE(arr.Size() == 6);
    REQUIRE(arr[2] == 99);
    REQUIRE(arr[3] == 2);  // Elements after the inserted one should shift
}

TEST_CASE(DynamicArray_Empty_Capacity) {
    DynamicArray<int> arr;

    REQUIRE(arr.Empty() == true);
    REQUIRE(arr.Capacity() == 0);

    arr.EmplaceBack(10);

    REQUIRE(arr.Empty() == false);
    REQUIRE(arr.Capacity() > 0);
}

TEST_CASE(DynamicArray_Move_Constructor) {
    DynamicArray<int> arr1(5);

    for (int i = 0; i < 5; ++i)
        arr1.EmplaceBack(i);

    DynamicArray<int> arr2(std::move(arr1));  // Move constructor

    REQUIRE(arr2.Size() == 5);
    REQUIRE(arr2.Capacity() == 5);
    REQUIRE(arr1.Size() == 0);  // arr1 should be empty after move
}

TEST_CASE(DynamicArray_Print_Function) {
    DynamicArray<int> arr;

    arr.EmplaceBack(1);
    arr.EmplaceBack(2);
    arr.EmplaceBack(3);

    std::ostringstream oss;
    oss << arr;

    REQUIRE(oss.str() == "{ 1, 2, 3 }");
}
