#pragma once

#include "Ocean/Core/PlatformDetection.hpp"

#ifdef OC_PLATFORM_WINDOWS
	#ifndef NOMINMAX
// See www.github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
		#define NOMINMAX
	#endif // !NOMINMAX
#endif // OC_PLATFORM_WINDOWS

#include <iostream>

#include <memory>
#include <utility>

#include <algorithm>
#include <functional>

#include <sstream>
#include <string>

#include <array>
#include <cstdint>
#include <vector>

#include <unordered_map>
#include <unordered_set>

#include "Ocean/Core/Base.hpp"

#include "Ocean/Debug/Instrumentor.hpp"

#ifdef OC_PLATFORM_WINDOWS
	#include <Windows.h>
#endif // OC_PLATFORM_WINDOWS
