#pragma once

#include "Ocean/Core/PlatformDetection.hpp"

#include <memory>
#include <intrin.h>
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
#endif

#define OC_EXPAND_MACRO(x) x
#define OC_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define OC_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Ocean {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) {
		std::cout << "Make Unique Ptr Call" << std::endl;

		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) {
		std::cout << "Make Shared Ptr Call" << std::endl;

		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}	// Ocean

#include "Ocean/Core/Log.hpp"
#include "Ocean/Core/Assert.hpp"
