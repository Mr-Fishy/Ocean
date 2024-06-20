#pragma once

#include "Ocean/Core/PlatformBase.hpp"

#include <iostream>

#ifdef OC_DEBUG
	#if defined(OC_PLATFORM_WINDOWS)
		#define OC_DEBUGBREAK() __debugbreak()
	#elif defined(OC_PLATFORM_LINUX)
		#include <signal.h>
		#define OC_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif

	#define OC_ENABLE_ASSERTS
#else
	#define OC_DEBUGBREAK()
#endif // OC_DEUBG

#define OC_EXPAND_MACRO(x) x
#define OC_STRINGIFY_MACRO(x) #x

#define OC_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#include "Ocean/Core/Log.hpp"
#include "Ocean/Core/Defines.hpp"
// #include "Ocean/Core/Assert.hpp"
