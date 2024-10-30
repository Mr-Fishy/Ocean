#pragma once

#include "Ocean/Core/Types/ValueTypes.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"

namespace Ocean {

	// Math Utils
	//
	// Conversions from float / double to int / uint

	// Define this macro to check if converted value can be contained in the destination.
	#define OCEAN_MATH_OVERFLOW_CHECK

	// Undefine the mac versions of this
	#undef max
	#undef min

	template <typename T>
	T max(const T& a, const T& b)                 { return a > b ? a : b; }

	template <typename T>
	T min(const T& a, const T& b)                 { return a < b ? a : b; }

	template <typename T>
	T clamp(const T& v, const T& a, const T& b)   { return v < a ? a : (v > b ? b : v); }


	template <typename To, typename From>
	To safe_cast(From a) {
		To result = static_cast<To>(a);

		From check = static_cast<From>(result);
		OASSERT(check == result);

		return result;
	}

	u32 ceilu32(f32 value);
	u32 ceilu32(f64 value);
	u16 ceilu16(f32 value);
	u16 ceilu16(f64 value);
	
	i32 ceili32(f32 value);
	i32 ceili32(f64 value);
	i16 ceili16(f32 value);
	i16 ceili16(f64 value);



	u32 flooru32(f32 value);
	u32 flooru32(f64 value);
	u16 flooru16(f32 value);
	u16 flooru16(f64 value);

	i32 floori32(f32 value);
	i32 floori32(f64 value);
	i16 floori16(f32 value);
	i16 floori16(f64 value);



	u32 roundu32(f32 value);
	u32 roundu32(f64 value);
	u16 roundu16(f32 value);
	u16 roundu16(f64 value);

	i32 roundi32(f32 value);
	i32 roundi32(f64 value);
	i16 roundi16(f32 value);
	i16 roundi16(f64 value);



	u32 absu32(u32 x);
	u16 absu16(u16 x);



	f32 get_random_value(f32 min, f32 max);



	



	// TODO: Trig Functions (Defined Table Optimization)



	const f32 opi  = 3.1415926538f;
	const f32 opi2 = 1.5707963267f;

}	// Ocean
