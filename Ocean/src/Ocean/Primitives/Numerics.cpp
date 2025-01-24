#include "Numerics.hpp"

// TODO: Type casting protection (out of bounds protection)

u32 ceilu32(f32 value) {
	return static_cast<u32>(value) + 1;
}

u32 ceilu32(f64 value) {
	return static_cast<u32>(value) + 1;
}

u16 ceilu16(f32 value) {
	return static_cast<u16>(value) + 1;
}

u16 ceilu16(f64 value) {
	return static_cast<u16>(value) + 1;
}



i32 ceili32(f32 value) {
	return static_cast<i32>(value) + 1;
}

i32 ceili32(f64 value) {
	return static_cast<i32>(value) + 1;
}

i16 ceili16(f32 value) {
	return static_cast<i16>(value) + 1;
}

i16 ceili16(f64 value) {
	return static_cast<i16>(value) + 1;
}



u32 flooru32(f32 value) {
	return static_cast<u32>(value);
}

u32 flooru32(f64 value) {
	return static_cast<u32>(value);
}

u16 flooru16(f32 value) {
	return static_cast<u16>(value);
}

u16 flooru16(f64 value) {
	return static_cast<u16>(value);
}



i32 floori32(f32 value) {
	return static_cast<i32>(value);
}

i32 floori32(f64 value) {
	return static_cast<i32>(value);
}

i16 floori16(f32 value) {
	return static_cast<i16>(value);
}

i16 floori16(f64 value) {
	return static_cast<i16>(value);
}



u32 roundu32(f32 value) {
	return static_cast<u32>(value);
}

u32 roundu32(f64 value) {
	return static_cast<u32>(value);
}

u16 roundu16(f32 value) {
	return static_cast<u16>(value);
}

u16 roundu16(f64 value) {
	return static_cast<u16>(value);
}



i32 roundi32(f32 value) {
	return static_cast<i32>(value);
}

i32 roundi32(f64 value) {
	return static_cast<i32>(value);
}

i16 roundi16(f32 value) {
	return static_cast<i16>(value);
}

i16 roundi16(f64 value) {
	return static_cast<i16>(value);
}



f32 get_random_value(f32 min, f32 max) {
	OASSERT(min < max);

	f32 rnd = static_cast<f32>(rand()) / static_cast<f32>(RAND_MAX);

	return (max - min) * rnd + min;
}



u32 factorial(u32 num) {
	u32 result = 1;
	for (u32 i = 2; i < num; i++)
		result *= i;

	return result;
}