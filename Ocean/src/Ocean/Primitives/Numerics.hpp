#pragma once

#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/FloatingPoints.hpp"

#include "Ocean/Primitives/Assert.hpp"

// Math Utils
//
// Conversions from float / double to int / uint

// Define this macro to check if converted value can be contained in the destination.
#define OCEAN_MATH_OVERFLOW_CHECK

// Undefine the mac versions of this
#undef max
#undef min

template <typename T>
T oMax(const T& a, const T& b)                 { return a > b ? a : b; }

template <typename T>
T oMin(const T& a, const T& b)                 { return a < b ? a : b; }

template <typename T>
T oClamp(const T& v, const T& a, const T& b)   { return v < a ? a : (v > b ? b : v); }


template <typename To, typename From>
To oSafe_cast(From a) {
	To result = static_cast<To>(a);

	From check = static_cast<From>(result);
	OASSERT(check == result);

	return result;
}

u32 oCeilu32(f32 value);
u32 oCeilu32(f64 value);
u16 oCeilu16(f32 value);
u16 oCeilu16(f64 value);

i32 oCeili32(f32 value);
i32 oCeili32(f64 value);
i16 oCeili16(f32 value);
i16 oCeili16(f64 value);



u32 oFlooru32(f32 value);
u32 oFlooru32(f64 value);
u16 oFlooru16(f32 value);
u16 oFlooru16(f64 value);

i32 oFloori32(f32 value);
i32 oFloori32(f64 value);
i16 oFloori16(f32 value);
i16 oFloori16(f64 value);



u32 oRoundu32(f32 value);
u32 oRoundu32(f64 value);
u16 oRoundu16(f32 value);
u16 oRoundu16(f64 value);

i32 oRoundi32(f32 value);
i32 oRoundi32(f64 value);
i16 oRoundi16(f32 value);
i16 oRoundi16(f64 value);



u32 oAbsu32(u32 x);
u16 oAbsu16(u16 x);



f32 oGetRandomValue(f32 min, f32 max);



sizet oFactorial(u8 num);





// TODO: Trig Functions (Defined Table Optimization)



constexpr f32 oPi  = 3.1415926538f;
constexpr f32 oPi2 = 1.5707963267f;

// f32 sinf32(f32 radians);
// f32 cosf32(f32 radians);
// f32 tanf32(f32 radians);