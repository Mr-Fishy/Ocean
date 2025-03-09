#pragma once

#include <cassert>
#include <functional>
#include <string>
#include <map>
#include <iostream>

class TestFactory {
public:
    static bool Register(std::string name, std::function<void()> func);

    static void Run();

private:
    static std::map<std::string, std::function<void()>> k_Tests;

};

#define TEST_CASE(name) \
    void test_##name (void); \
    static bool test_##name##_registered = TestFactory::Register(#name, &test_##name); \
    void test_##name (void)
    
#define REQUIRE(condition) \
    assert((condition))

#define CONSOLE_OUT \
    std::cerr

#define RUN_TESTS() \
    TestFactory::Run()

#define MAIN \
    int main(int argc, char** argv)
