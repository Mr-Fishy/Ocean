#pragma once

#include "Ocean/Core/Types/ComplexTypeQualifier.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"

typedef vec<2, b8>  bvec2;

typedef vec<2, u32> uvec2;

typedef vec<2, i32> ivec2;

typedef vec<2, f64> dvec2;

typedef vec<2, f32> fvec2;

/**
 * @brief A 2D vector of the set type.
 * @tparam T - The type of numeric to use.
 * @tparam P - The precision to use in calulations.
 */
template<typename T, Precision P>
struct vec<2, T, P> {
	/* --- Details --- */

	typedef T value_type;
	typedef vec<2, T> type;
	typedef vec<2, b8> bool_type;

	enum is_aligned {
		value = false
	};

	/* --- Data --- */

	union { T x, r, s; };
	union { T y, g, t; };

	/* --- Component Access --- */
	
	typedef u8 length_type;
	/**
	 * @return The number of components in the vector.
	 */
	static length_type length() { return 2; }

	T& operator [] (length_type i) {
		OASSERT_LENGTH(i, this->length());

		switch (i) {
			default:
			case 0:
				return x;
			case 1:
				return y;
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
		}
	}

	/* --- Implicit Constructors --- */

	vec() = default;
	vec(vec const& v) = default;

	template<Precision P>
	vec(vec<2, T, P> const& v) : x(v.x), y(v.y) { }

	/* --- Explicit Constructors --- */

	explicit vec(T scalar) : x(scalar), y(scalar) { }

	vec(T x, T y) : x(x), y(y) { }

	/* --- Conversion Constructors --- */

	

	/* --- Conversion Vector Constructors --- */



	/* --- Unary Arithmetic Operators --- */



	/* --- Increment & Decrement Operators --- */



	/* --- Unary Bit Operators --- */



};	// vec2

/* --- Unary Operators --- */



/* --- Binary Operators --- */

