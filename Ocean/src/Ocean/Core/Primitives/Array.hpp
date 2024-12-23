#pragma once

#include "Ocean/Core/Types/Integers.hpp"

// std
#include <cstring>
#include <vector>


namespace Ocean {

	template <class T>
	using DynamicArray = std::vector<T>;

	template <class T, sizet S>
	using FixedArray = std::array<T, S>;

}	// Ocean
