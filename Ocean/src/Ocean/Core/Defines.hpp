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

// Macros

#define OC_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
