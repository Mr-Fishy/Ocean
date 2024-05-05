#pragma once

#include "Ocean/Memory/Allocator.hpp"

namespace Ocean {

	class LinearAlloc : public Allocator {
	public:
		LinearAlloc(const size_t initSize) noexcept;
		virtual ~LinearAlloc() noexcept;

		virtual void Reset() override;

		virtual void* Alloc(const size_t& size, MemTypeTags tag = MemTag::UNKOWN, const size_t alignment = 0) override;
		virtual void Free(const void* ptr) override;

		virtual void ResizePool() override;

		void* GetStart() const noexcept { return m_Start; }

	private:
		void* m_Start = nullptr;

		size_t m_Offset = 0;
	};

}	// Ocean
