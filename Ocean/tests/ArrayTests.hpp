#pragma once

/**
 * @file ArrayTests.hpp
 * @author Evan F.
 * @brief This file is to organize all Array.hpp related tests.
 * @date 2025-01-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <Ocean/Ocean.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>



TEST_CASE("Dynamic Arrays", "[Arrays]") {

    SECTION("Construction and Initialization") {

        DynamicArray<u32> vec1;
        REQUIRE( vec1.size() == 0 );

        DynamicArray<u32> vec2(5);
        REQUIRE( vec2.size() == 5 );

        for (u32 i = 0; i < vec2.size(); i++)
            REQUIRE( vec2[i] == 0 );

        DynamicArray<u32> vec3(6, 10);
        REQUIRE( vec3.size() == 6 );

        for (u32 i = 0; i < vec3.size(); i++)
            REQUIRE( vec3[i] == 10 );

    }

    SECTION("Element Access") {

        DynamicArray<u32> vec = { 1, 2, 3, 4, 5 };

        REQUIRE( vec[0] == 1 );
        REQUIRE( vec[4] == 5 );

        REQUIRE( vec.at(3) == 4 );
        REQUIRE( vec.at(1) == 2 );

        REQUIRE( vec.front() == 1 );
        REQUIRE( vec.back() == 5 );

    }

    SECTION("Modification") {

        DynamicArray<u32> vec = { 1, 2, 3 };

        vec.push_back(4);
        REQUIRE( vec.size() == 4 );
        REQUIRE( vec.back() == 4 );

        vec.pop_back();
        REQUIRE( vec.size() == 3 );
        REQUIRE(vec.back() == 3);

        vec.insert(vec.begin() + 1, 10);
        REQUIRE(vec.size() == 4);
        REQUIRE(vec[1] == 10);

        vec.erase(vec.begin() + 1);
        REQUIRE(vec.size() == 3);
        REQUIRE(vec[1] == 2);

    }

    SECTION("Resizing") {

        DynamicArray<u32> vec = { 1, 2, 3 };

        vec.resize(5);
        REQUIRE( vec.size() == 5 );

        INFO("New Elements Should Be Default Initialized");
        REQUIRE( vec[3] == 0 );
        REQUIRE( vec[4] == 0 );

        vec.resize(2);
        REQUIRE( vec.size() == 2 );

    }

    SECTION("Iterators") {

        DynamicArray<u32> vec = { 1, 2, 3, 4, 5 };

        u32 sum = 0;
        for (auto it = vec.begin(); it != vec.end(); it++)
            sum += *it;

        REQUIRE(sum == 15);

        sum = 0;
        for (auto it = vec.rbegin(); it != vec.rend(); it++)
            sum += *it;

        REQUIRE(sum == 15);

    }

    SECTION("Clear and Empty") {

        DynamicArray<u32> vec = {1, 2, 3};

        REQUIRE_FALSE(vec.empty());

        vec.clear();
        REQUIRE(vec.empty());
        REQUIRE(vec.size() == 0);

    }

    SECTION("Copy and Move Semantics") {

        DynamicArray<u32> vec1 = { 1, 2, 3 };
        DynamicArray<u32> vec2 = vec1; // Copy constructor

        REQUIRE(vec2.size() == 3);
        REQUIRE(vec2[0] == 1);
        
        DynamicArray<u32> vec3 = std::move(vec1); // Move constructor
        REQUIRE(vec3.size() == 3);
        REQUIRE(vec3[0] == 1);
        REQUIRE(vec1.empty()); // vec1 should be empty after move

    }

    SECTION("Assignment Operators") {

        DynamicArray<u32> vec1 = {1, 2, 3};
        DynamicArray<u32> vec2;
        
        vec2 = vec1; // Copy assignment
        REQUIRE(vec2.size() == 3);
        REQUIRE(vec2[0] == 1);

        DynamicArray<u32> vec3;
        vec3 = std::move(vec1); // Move assignment
        REQUIRE(vec3.size() == 3);
        REQUIRE(vec3[0] == 1);
        REQUIRE(vec1.empty()); // vec1 should be empty after move

    }

}

TEST_CASE("Dynamic Array Benchmarks", "[Arrays]") {

    BENCHMARK("push_back") {
        DynamicArray<u32> vec;
        for (u32 i = 0; i < 10000; ++i) {
                vec.push_back(i);
        }
    };

    BENCHMARK("pop_back") {
        DynamicArray<u32> vec;
        for (u32 i = 0; i < 10000; ++i) {
            vec.push_back(i);
        }
        for (u32 i = 0; i < 10000; ++i) {
            vec.pop_back();
        }
    };

    BENCHMARK("Insert At Beginning") {
        DynamicArray<u32> vec;
        for (u32 i = 0; i < 10000; ++i) {
            vec.push_back(i);
        }
        for (u32 i = 0; i < 10000; ++i) {
            vec.insert(vec.begin(), i);
        }
    };

    BENCHMARK("Erase From Beginning") {
        DynamicArray<u32> vec;
        for (u32 i = 0; i < 10000; ++i) {
            vec.push_back(i);
        }
        for (u32 i = 0; i < 10000; ++i) {
            vec.erase(vec.begin());
        }
    };

    BENCHMARK("Resize") {
        DynamicArray<u32> vec;
        vec.resize(10000);
    };

    BENCHMARK("Access") {
        DynamicArray<u32> vec(10000);
        for (u32 i = 0; i < 10000; ++i) {
            vec[i] = i;
        }
        u32 sum = 0;
        for (u32 i = 0; i < 10000; ++i) {
            sum += vec[i];
        }
    };

    BENCHMARK("Clear") {
        DynamicArray<u32> vec;
        for (u32 i = 0; i < 10000; ++i) {
            vec.push_back(i);
        }
        vec.clear();
    };
}
