#pragma once

#include <memory>

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/* Windows x64  */
		#define OC_PLATFORM_WINDOWS
	#else
		/* Windows x86 */
		#error "x86 Builds are not supported!"
#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
#if TARGET_IPHONE_SIMULATOR == 1
	#error "IOS simulator is not supported!"
#elif TARGET_OS_IPHONE == 1
	#define OC_PLATFORM_IOS
	#error "IOS is not supported!"
#elif TARGET_OS_MAC == 1
	#define OC_PLATFORM_MACOS
	#error "MacOS is not supported!"
#else
	#error "Unknown Apple platform!"
#endif
 /* We also have to check __ANDROID__ before __linux__
  * since android is based on the linux kernel
  * it has __linux__ defined */
#elif defined(__ANDROID__)
	#define OC_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define OC_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	/* Unknown compiler/platform */
	#error "Unknown platform!"
#endif // End of platform detection


// DLL support
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
	#error "Ocean only supports Windows!"
#endif // End of DLL support

#ifdef OC_DEBUG
	#define OC_ENABLE_ASSERTS
#endif

#ifdef OC_ENABLE_ASSERTS
	#define OC_ASSERT(x, ...) { if(!(x)) { OC_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define OC_CORE_ASSERT(x, ...) { if(!(x)) { OC_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define OC_ASSERT(x, ...)
	#define OC_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define OC_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Ocean {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}
