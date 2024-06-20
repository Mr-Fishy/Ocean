#pragma once

// TODO: Bit-Size Checks Should Exist

// Unsigned Integer Types

/**
 * @brief Unsigned 8-Bit integer.
 */
typedef unsigned char ui8;

/**
 * @brief Unsigned 16-Bit integer;
 */
typedef unsigned short ui16;

/**
 * @brief Unsigned 32-Bit integer;
 */
typedef unsigned int ui32;

/**
 * @brief Unsigned 64-Bit integer;
 */
typedef unsigned long long ui64;

// Signed Integer Types

/**
 * @brief Signed 8-Bit integer.
 */
typedef signed char i8;

/**
 * @brief Signed 16-Bit integer;
 */
typedef signed short i16;

/**
 * @brief Signed 32-Bit integer;
 */
typedef signed int i32;

/**
 * @brief Signed 64-Bit integer;
 */
typedef signed long long i64;

// Floating Point Types

/**
 * @brief 32-Bit floating point number.
 */
typedef float f32;

/**
 * @brief 64-Bit floating point number.
 */
typedef double f64;

// Boolean Types

/**
 * @brief 8-Bit boolean value.
 */
typedef bool b8;

/**
 * @brief 32-Bit boolean value.
 */
typedef int b32;

#include <memory>

namespace Ocean {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) {
		// std::cout << "Make Unique Ptr Call" << std::endl;

		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) {
		// std::cout << "Make Shared Ptr Call" << std::endl;

		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}	// Ocean
