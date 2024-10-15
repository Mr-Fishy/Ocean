#include "Numerics.hpp"

namespace Ocean {

	u32 ceilu32(f32 value) {
		return u32();
	}

	u32 ceilu32(f64 value) {
		return u32();
	}

	u16 ceilu16(f32 value) {
		return u16();
	}

	u16 ceilu16(f64 value) {
		return u16();
	}



	i32 ceili32(f32 value) {
		return i32();
	}

	i32 ceili32(f64 value) {
		return i32();
	}

	i16 ceili16(f32 value) {
		return i16();
	}

	i16 ceili16(f64 value) {
		return i16();
	}



	u32 flooru32(f32 value) {
		return u32();
	}

	u32 flooru32(f64 value) {
		return u32();
	}

	u16 flooru16(f32 value) {
		return u16();
	}

	u16 flooru16(f64 value) {
		return u16();
	}



	i32 floori32(f32 value) {
		return i32();
	}

	i32 floori32(f64 value) {
		return i32();
	}

	i16 floori16(f32 value) {
		return i16();
	}

	i16 floori16(f64 value) {
		return i16();
	}



	u32 roundu32(f32 value) {
		return u32();
	}

	u32 roundu32(f64 value) {
		return u32();
	}

	u16 roundu16(f32 value) {
		return u16();
	}

	u16 roundu16(f64 value) {
		return u16();
	}



	i32 roundi32(f32 value) {
		return i32();
	}

	i32 roundi32(f64 value) {
		return i32();
	}

	i16 roundi16(f32 value) {
		return i16();
	}

	i16 roundi16(f64 value) {
		return i16();
	}



	f32 get_random_value(f32 min, f32 max) {
		OASSERT(min < max);

		f32 rnd = (f32)rand() / (f32)RAND_MAX;

		return (max - min) * rnd + min;
	}

}	// Ocean
