#include "Bit.hpp"

#include "Ocean/Core/Primitives/Log.hpp"
#include "Ocean/Core/Primitives/Memory.hpp"

#if defined(_MSC_VER)

	#include <immintrin.h>
	#include <intrin0.h>

#endif


namespace Ocean {

	u32 oLeadingZeroesu32(u32 x) {
	#if defined (_MSC_VER)

		return __lzcnt(x);

	#else

		return __builtin_clz(x);

	#endif
	}

	u32 oTrailingZeroesu32(u32 x) {
	#if defined (_MSC_VER)

		return _tzcnt_u32(x);

	#else

		return __builtin_ctz(x);

	#endif
	}

	u64 oTrailingZeroesu64(u64 x) {
	#if defined (_MSC_VER)

		return _tzcnt_u64(x);

	#else

		return __builtin_ctzl(x);

	#endif
	}

	u32 oRoundPowerOf2(u32 v) {
		u32 nv = 1 << (32 - oLeadingZeroesu32(v));

		return nv;
	}

	void oPrintBinary(u64 n) {
		oprint("0b");
		for (u8 i = 0; i < 64; i++) {
			u64 bit = (n >> (64 - i - 1)) & 0x1;
			oprint("%llu", bit);
		}
		oprint(" ");
	}

	void oPrintBinary(u32 n) {
		oprint("0b");
		for (u8 i = 0; i < 64; i++) {
			u64 bit = (n >> (64 - i - 1)) & 0x1;
			oprint("%llu", bit);
		}
		oprint(" ");
	}



	void BitSet::Init(Allocator* allocator, u32 totalBits) {
		p_Allocator = allocator;
		p_Bits = nullptr;
		m_Size = 0;

		Resize(totalBits);
	}

	void BitSet::Shutdown() {
		ofree(p_Bits, p_Allocator);
	}

	void BitSet::Resize(u32 totalBits) {
		u8* oldBits = p_Bits;

		const u32 newSize = (totalBits + 7) / 8;
		if (m_Size == newSize)
			return;

		p_Bits = (u8*)oallocam(newSize, p_Allocator);

		if (oldBits) {
			mem_copy(p_Bits, oldBits, m_Size);
			ofree(oldBits, p_Allocator);
		}
		else {
			memset(p_Bits, 0, newSize);
		}

		m_Size = newSize;
	}

}	// Ocean
