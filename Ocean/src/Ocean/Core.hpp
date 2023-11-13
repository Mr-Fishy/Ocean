#pragma once

// Ocean DLL Macros
#ifdef OC_PLATFORM_WINDOWS
	#if OC_DYNAMIC_LINK
		#ifdef OC_BUILD_DLL
			#define OCEAN_API __declspec(dllexport)
		#else
			#define OCEAN_API __declspec(dllimport)
		#endif
	#else
		#define OCEAN_API
	#endif
#else
	#error Ocean only supports Windows!
#endif

#ifdef OC_DEBUG
	#define OC_ENABLE_ASSERTS
#endif

// Assertion Macros
#ifdef OC_ENABLE_ASSERTS
	#define OC_ASSERT(x, ...) { if(!(x)) { OC_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define OC_CORE_ASSERT(x, ...) { if(!(x)) { OC_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define OC_ASSERT(x, ...)
	#define OC_CORE_ASSERT(x, ...)
#endif // OC_ENABLE_ASSERTS

// Bit Location Macro
#define BIT(x) (1 << x)

// Event Binding Macro
#define OC_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
