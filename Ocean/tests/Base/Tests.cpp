#include "Tests.hpp"

// std
#include <iostream>

bool TestFactory::Register(std::string name, std::function<void()> func) {
    auto it = k_Tests.find(name);

    if (it == k_Tests.end()) {
        k_Tests[name] = std::move(func);
        return true;
    }

    return false;
}

void TestFactory::Run() {
    std::cerr << "\n========================================\n";
    std::cerr << "Ocean Test Output:\n";

    for (const auto& func : k_Tests) {
        std::cerr << "\tRunning Test Case: " << func.first << std::endl;

        func.second();
    }

    std::cerr << "========================================\n";
    std::cerr << std::endl;
}

std::map<std::string, std::function<void()>> TestFactory::k_Tests;

MAIN { RUN_TESTS(); }
