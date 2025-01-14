#pragma once

#include "Ocean/Types/Integers.hpp"

// std
#include <vector>
#include <array>


template <class T>
using DynamicArray = std::vector<T>;

template <class T, sizet S>
using FixedArray = std::array<T, S>;
