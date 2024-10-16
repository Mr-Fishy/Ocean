#pragma once

#include "Ocean/Core/Types/ComplexTypeQualifier.hpp"
#include "Ocean/Core/Types/Vector4D.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"

typedef mat<4, 4, b8>  bmat4;

typedef mat<4, 4, u32> umat4;

typedef mat<4, 4, i32> imat4;

typedef mat<4, 4, f64> dmat4;

typedef mat<4, 4, f32> fmat4;

template<typename T, Precision P>
struct mat<4, 4, T, P> {
	/* --- Details --- */

	typedef T value_type;
	typedef vec<4, T, P> col_type;
	typedef vec<4, T, P> row_type;
	typedef mat<4, 4, T, P> type;
	typedef mat<4, 4, T, P> transpose_type;

private:
	/* --- Data --- */

	col_type value[4];

public:
	/* --- Component Access --- */

	typedef u8 length_type;
	/**
	 * @return The number of components in a row;
	 */
	static length_type length() { return 4; }
	/**
	 * @return The number of components in a column.
	 */
	static length_type height() { return 4; }

	T& operator [] (length_type i) {
		OASSERT_LENGTH(i, this->length());

		return this->value[i];
	}
	T const& operator [] (length_type i) const {
		OASSERT_LENGTH(i, this->length());

		return this->value[i];
	}

	/* --- Implicit Constructors --- */



	/* --- Explicit Constructors --- */



	/* --- Conversion Constructors --- */



	/* --- Conversion Vector Constructors --- */



	/* --- Unary Arithmetic Operators --- */



	/* --- Increment & Decrement Operators --- */



	/* --- Unary Bit Operators --- */



};	// vec2

/* --- Unary Operators --- */



/* --- Binary Operators --- */
