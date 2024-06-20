#pragma once

#include "Ocean/Core/Base.hpp"
#include "Ocean/Core/Log.hpp"

// std
#include <filesystem>

#ifdef OC_ENABLE_ASSERTS
	// Alternative 'default' message for both "WITH_MSG" and "NO_MSG"
	// Which provides support for custom formatting by concatenating the formatting string instead of have the format inside the default message
	#define OC_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { \
																	OC##type##ERROR(msg, __VA_ARGS__); OC_DEBUGBREAK(); \
																} }

	#define OC_INTERNAL_ASSERT_WITH_MSG(type, check, msg) \
					OC_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: " + msg)

	#define OC_INTERNAL_ASSERT_NO_MSG(type, check) \
					OC_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", OC_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define OC_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro

	#define OC_INTERNAL_ASSERT_GET_MACRO(...) \
					OC_EXPAND_MACRO( OC_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, OC_INTERNAL_ASSERT_WITH_MSG, OC_INTERNAL_ASSERT_NO_MSG) )

		// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define OC_ASSERT(...) \
					OC_EXPAND_MACRO( OC_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )

	#define OC_CORE_ASSERT(...) \
					OC_EXPAND_MACRO( OC_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define OC_ASSERT(...)
	#define OC_CORE_ASSERT(...)
#endif // OC_ENABLE_ASSERTS
