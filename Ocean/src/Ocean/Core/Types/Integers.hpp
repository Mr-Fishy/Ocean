#pragma once

// Unsigned Integers

/**
 * @brief 64 bit unsigned integer.
 */
typedef unsigned long long u64;
typedef u64 sizet;

static const u64 u64_max = 0xffffffffffffffffui64;

/**
 * @brief 32 bit unsigned integer.
 */
typedef unsigned int u32;

static const u32 u32_max = 0xffffffffui32;

/**
 * @brief 16 bit unsigned integer.
 */
typedef unsigned short u16;

static const u16 u16_max = 0xffffui16;

/**
 * @brief 8 bit unsigned integer.
 */
typedef unsigned char u8;

static const u8 u8_max = 0xffui8;

// Signed Integers

/**
 * @brief 64 bit integer.
 */
typedef long long i64;

static const i64 i64_max = 9223372036854775807i64;
static const i64 i64_min = (-9223372036854775807i64 - 1);

/**
 * @brief 32 bit integer.
 */
typedef int i32;

static const i32 i32_max = 2147483647i32;
static const i32 i32_min = (-2147483647i32 - 1);

/**
 * @brief 16 bit integer.
 */
typedef short i16;

static const i16 i16_max = 32767i16;
static const i16 i16_min = (-32767i16 - 1);

/**
 * @brief 8 bit integer.
 */
typedef char i8;

static const i8 i8_max = 127i8;
static const i8 i8_min = (-127i8 - 1);
