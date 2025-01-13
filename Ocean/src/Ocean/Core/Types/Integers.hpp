#pragma once

#include <cstdint>

// Unsigned Integers

/**
 * @brief 64 bit unsigned integer.
 */
typedef unsigned long long u64;
typedef std::size_t sizet;

static const u64 u64_max = 0xffffffffffffffffUL;

/**
 * @brief 32 bit unsigned integer.
 */
typedef std::uint32_t u32;

static const u32 u32_max = 0xffffffffU;

/**
 * @brief 16 bit unsigned integer.
 */
typedef std::uint16_t u16;

static const u16 u16_max = 0xffffU;

/**
 * @brief 8 bit unsigned integer.
 */
typedef std::uint8_t u8;

static const u8 u8_max = 0xffU;

// Signed Integers

/**
 * @brief 64 bit integer.
 */
typedef std::int64_t i64;

static const i64 i64_max = 9223372036854775807L;
static const i64 i64_min = (-9223372036854775807L - 1);

/**
 * @brief 32 bit integer.
 */
typedef std::int32_t i32;

static const i32 i32_max = 2147483647;
static const i32 i32_min = (-i32_max - 1);

/**
 * @brief 16 bit integer.
 */
typedef std::int16_t i16;

static const i16 i16_max = 32767;
static const i16 i16_min = (-i16_max - 1);

/**
 * @brief 8 bit integer.
 */
typedef std::int8_t i8;

static const i8 i8_max = 127;
static const i8 i8_min = (-i8_max - 1);
