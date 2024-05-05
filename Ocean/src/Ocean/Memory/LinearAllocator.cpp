
#include "ocpch.hpp"
#include "LinearAllocator.hpp"

namespace Ocean {

	LinearAlloc::LinearAlloc(const size_t initSize) noexcept
		: Allocator(initSize) {
		if (m_Start != nullptr)
			free(m_Start);

		m_Start = malloc(m_Size);
		m_Offset = 0;
	}

	LinearAlloc::~LinearAlloc() noexcept {
		OC_CORE_ASSERT(m_AllocCount == 0 && m_Used == 0, "LinearAlloc::~LinearAlloc() Not all allocated memory has been freed!");

		memset(m_Start, 0, m_Size); // Clear any data in memory before releasing it.
		free(m_Start);				// Free the memory block.

		m_Start = nullptr;
	}

	void LinearAlloc::Reset() {
		m_Used = 0;
		m_Peak = 0;
		m_Offset = 0;
		m_AllocCount = 0;

		memset(m_Start, 0, m_Size); // Clear the memory block data.
	}

	void* LinearAlloc::Alloc(const size_t& size, MemTypeTags tag, const size_t alignment) {
		OC_CORE_ASSERT(size > 0 && alignment >= 0, "LinearAlloc::Alloc() Has been given an invalid size or alignment value!");

		const size_t base = reinterpret_cast<size_t>(m_Start) + m_Offset;
		size_t padding = 0;
		size_t aligned = 0;

		if (alignment != 0 && m_Offset % alignment != 0)
			padding = Internal::Mem::CalculatePadding(base, alignment);

		if (m_Used + padding + size > m_Size)
			throw std::bad_alloc();

		const size_t next = base + padding;

		m_Offset += padding;
		m_Offset += size;
		m_AllocCount++;

		m_Used = m_Offset;
		m_UsedList[tag] += size + padding;
		m_Peak = std::max(m_Peak, m_Used);

		memset(reinterpret_cast<void*>(next), 0, size); // Clear the allocated data before giving the memory back.

		return reinterpret_cast<void*>(next);
	}

	void LinearAlloc::Free(const void* ptr) {
		OC_CORE_ASSERT(false, "Cannot free from linear allocator! Use LinearAlloc::Reset()");
	}

	void LinearAlloc::ResizePool() {
		OC_CORE_ASSERT(false, "LinearAlloc::ResizePool() Is not implemented!");
	}

}	// Ocean
