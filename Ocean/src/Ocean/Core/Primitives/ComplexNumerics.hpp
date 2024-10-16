#pragma once

#include "Ocean/Core/Types/Matricies.hpp"

#include "Ocean/Core/Primitives/Numerics.hpp"

template<u8 L, typename T, Precision P>
inline vec<L, T, P> normalize(vec<L, T, P> const& x) {
	return 
}

// translate

template<typename T, Precision P>
inline mat<4, 4, T, P> rotate(mat<4, 4, T, P> const& m, T angle, vec<3, T, P> const& axis) {
	return v * 
}

// scale

// shear

template<typename T, Precision P>
mat<4, 4, T, P> lookAt(vec<3, T, P> const& eye, T angle, vec<3, T, P> const& axis) {
	vec<3, T, P> const f();
	vec<3, T, P> const s();
	vec<3, T, P> const u();

	return {  };
}
