#pragma once

#include "Ocean/Core/Types/ValueTypes.hpp"

namespace Ocean {

	class Allocator;

	// Methods

	u32 oLeadingZeroesu32(u32 x);

	u32 oTrailingZeroesu32(u32 x);
	u64 oTrailingZeroesu64(u64 x);

	u32 oRoundPowerOf2(u32 v);

	void oPrintBinary(u64 n);
	void oPrintBinary(u32 n);

	// BitMask

	template <class T, i32 Bits, i32 Shift = 0>
	class BitMask {
	public:
		using value_type = i32;
		using iterator = BitMask;
		using const_iterator = BitMask;

		explicit BitMask(T mask) : m_Mask(mask) { }

		BitMask& operator ++   () { m_Mask &= (m_Mask - 1); return *this; }
		explicit operator bool () const { return m_Mask != 0; }
		i32      operator *    () const { return LowestBitSet(); }

		u32 LowestBitSet() const { return oTrailingZeroesu32(m_Mask) >> Shift; }
		u32 HighestBitSet() const { return static_cast<u32>(bit_width(m_Mask) - 1) >> Shift; }

		BitMask Begin() const { return *this; }
		BitMask End() const { return BitMask(0); }

		u32 LeadingZeroes() const { return oLeadingZeroesu32(m_Mask); }
		u32 TrailingZeroes() const { return oTrailingZeroesu32(m_Mask); }

	private:
		friend b8 operator == (const BitMask& a, const BitMask& b) { return a.m_Mask == b.m_Mask; }
		friend b8 operator != (const BitMask& a, const BitMask& b) { return a.m_Mask != b.m_Mask; }

		T m_Mask;

	};	// BitMask

	// Utility Methods

	inline u32 oBitMask8(u32 bit) { return 1 << (bit & 7); }
	inline u32 oBitSlot8(u32 bit) { return bit / 8; }

	class BitSet {
	public:
		void Init(Allocator* allocator, u32 totalBits);
		void Shutdown();

		void Resize(u32 totalBits);

		void SetBit(u32 index) { p_Bits[index / 8] |= oBitMask8(index); }
		void ClearBit(u32 index) { p_Bits[index / 8] &= ~oBitMask8(index); }
		u8 GetBit(u32 index) { return p_Bits[index / 8] & oBitMask8(index); }

	private:
		Allocator* p_Allocator = nullptr;

		u8* p_Bits = nullptr;

		u32 m_Size = 0;

	};	// BitSet

	template <u32 SizeInBytes>
	class BitSetFixed {
	public:
		void SetBit(u32 index) { bits[index / 8] |= oBitMask8(index); }
		void ClearBit(u32 index) { bits[index / 8] &= ~oBitMask8(index); }
		u8 GetBit(u32 index) { return bits[index / 8] & oBitMask8(index); }

	private:
		u8 m_Bits[SizeInBytes];

	};	// BitSetFixed

}	// Ocean
