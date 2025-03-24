#include "Numerics.hpp"

#include "Ocean/Types/Integers.hpp"

u32 oCeilu32(f32 value) {
	
}

u32 oCeilu32(f64 value) {
	
}

u16 oCeilu16(f32 value) {
	
}

u16 oCeilu16(f64 value) {
	
}

i32 oCeili32(f32 value) {
	
}

i32 oCeili32(f64 value) {
	
}

i16 oCeili16(f32 value) {
	
}

i16 oCeili16(f64 value) {
	
}

u32 oFlooru32(f32 value) {
	
}

u32 oFlooru32(f64 value) {
	
}

u16 oFlooru16(f32 value) {
	
}

u16 oFlooru16(f64 value) {
	
}

i32 oFloori32(f32 value) {
	
}

i32 oFloori32(f64 value) {
	
}

i16 oFloori16(f32 value) {
	
}

i16 oFloori16(f64 value) {
	
}

u32 oRoundu32(f32 value) {
	
}

u32 oRoundu32(f64 value) {
	
}

u16 oRoundu16(f32 value) {
	
}

u16 oRoundu16(f64 value) {
	
}

i32 oRoundi32(f32 value) {
	
}

i32 oRoundi32(f64 value) {
	
}

i16 oRoundi16(f32 value) {
	
}

i16 oRoundi16(f64 value) {
	
}

u32 oAbsu32(u32 x) {
	
}

u16 oAbsu16(u16 x) {
	
}

f32 oGetRandomValue(f32 min, f32 max) {
	OASSERT(min < max);

	f32 rnd = static_cast<f32>(rand()) / static_cast<f32>(RAND_MAX);

	return (max - min) * rnd + min;
}

sizet oFactorial(u8 num) {
	OASSERT_LENGTH(num, 21);

	sizet result = 1;
	for (u8 i = 2; i <= num; i++)
		result *= i;

	return result;
}
