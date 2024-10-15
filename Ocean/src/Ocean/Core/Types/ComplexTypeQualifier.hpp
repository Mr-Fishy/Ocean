#pragma once

#include "Ocean/Core/Types/ValueTypes.hpp"

enum TypeQualifier {
	PACKED_HIGH_PRECISION,
	PACKED_MEDIUM_PRECISION,
	PACKED_LOW_PRECISION,

	HIGH_PRECISION = PACKED_HIGH_PRECISION,
	MEDIUM_PRECISION = PACKED_MEDIUM_PRECISION,
	LOW_PRECISION = PACKED_LOW_PRECISION,

	DEFAULT = HIGH_PRECISION
};

typedef TypeQualifier Precision;

template<u8 L, typename T, Precision P = Precision::DEFAULT> struct vec;
template<u8 C, u8 R, typename T, Precision P = Precision::DEFAULT> struct mat;
template<typename T, Precision P = Precision::DEFAULT> struct qua;


namespace Detail {

	template<Precision P>
	struct IsAligned {
		static const b8 value = false;
	};	// IsAligned


	template<u8 L, typename T, b8 is_aligned>
	struct Storage {
		typedef struct Type {
			T data[L];
		} Type;
	};	// Storage

	template<u8 L, typename T>
	struct Storage<L, T, true> {
		typedef struct alignas(L * sizeof(T)) Type {
			T data[L];
		} Type;
	};	// Storage<L, T, true>

}	// Detail
