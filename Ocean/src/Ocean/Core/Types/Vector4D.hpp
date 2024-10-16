#pragma once

#include "Ocean/Core/Types/ComplexTypeQualifier.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"

typedef vec<4, b8>  bvec4;

typedef vec<4, u32> uvec4;

typedef vec<4, i32> ivec4;

typedef vec<4, f64> dvec4;

typedef vec<4, f32> fvec4;

/**
 * @brief A 3D vector of the set type.
 * @tparam T - The type of numeric to use.
 * @tparam P - The precision to use in calulations.
 */
template<typename T, Precision P>
struct vec<4, T, P> {
	/* --- Details --- */

	typedef T value_type;
	typedef vec<4, T, P> type;
	typedef vec<4, b8, P> bool_type;

	enum is_aligned {
		value = Detail::IsAligned<P>::value
	};

	/* --- Data --- */

	union { T x, r; };
	union { T y, g; };
	union { T z, b; };
	union { T w, a; };

	/* --- Component Access --- */

	typedef u8 length_type;
	/**
	 * @return The number of components in the vector.
	 */
	static length_type length() { return 4; }

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
			case 3:
				return w;
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
			case 3:
				return w;
		}
	}

	/* --- Implicit Constructors --- */

	vec() = default;
	vec(vec const& v) = default;

	template<Precision P>
	vec(vec<4, T, P> const& v) : x(v.x), y(v.y), z(v.z), w(v.w) { }

	/* --- Explicit Constructors --- */

	explicit vec(T scalar) : x(scalar), y(scalar), z(scalar), w(scalar) { }

	vec(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) { }

	/* --- Conversion Constructors --- */



	/* --- Conversion Vector Constructors --- */



	/* --- Unary Arithmetic Operators --- */



	/* --- Increment & Decrement Operators --- */



	/* --- Unary Bit Operators --- */



};	// vec2

/* --- Unary Operators --- */



/* --- Binary Operators --- */

