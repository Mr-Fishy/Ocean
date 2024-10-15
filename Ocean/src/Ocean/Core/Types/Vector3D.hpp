#pragma once

#include "Ocean/Core/Types/ComplexTypeQualifier.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"

typedef vec<3, b8>  bvec3;

typedef vec<3, u32> uvec3;

typedef vec<3, i32> ivec3;

typedef vec<3, f64> dvec3;

typedef vec<3, f32> fvec3;

/**
 * @brief A 3D vector of the set type.
 * @tparam T - The type of numeric to use.
 * @tparam P - The precision to use in calulations.
 */
template<typename T, Precision P>
struct vec<3, T, P> {
	/* --- Details --- */

	typedef T value_type;
	typedef vec<3, T> type;
	typedef vec<3, b8> bool_type;

	enum is_aligned {
		value = Detail::IsAligned<P>::value
	};

	/* --- Data --- */

	union { T x, r, s; };
	union { T y, g, t; };
	union { T z, b, p; };

	/* --- Component Access --- */

	typedef u8 length_type;
	/**
	 * @return The number of components in the vector.
	 */
	static length_type length() { return 3; }

	T& operator [] (length_type i) {
		OASSERT_LENGTH(i, this->length());

		switch (i) {
			default:
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
		}
	}
	T const& operator [] (length_type i) const {
		OASSERT_LENGTH(i, this->length());

		switch (i) {
			default:
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
		}
	}

	/* --- Implicit Constructors --- */

	vec() = default;
	vec(vec const& v) = default;

	template<Precision P>
	vec(vec<3, T, P> const& v) : x(v.x), y(v.y), z(v.z) { }

	/* --- Explicit Constructors --- */

	explicit vec(T scalar) : x(scalar), y(scalar), z(scalar) { }

	vec(T x, T y, T z) : x(x), y(y), z(z) { }

	/* --- Conversion Constructors --- */



	/* --- Conversion Vector Constructors --- */



	/* --- Unary Arithmetic Operators --- */



	/* --- Increment & Decrement Operators --- */



	/* --- Unary Bit Operators --- */



};	// vec2

/* --- Unary Operators --- */



/* --- Binary Operators --- */

