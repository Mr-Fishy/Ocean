
#include "ocpch.hpp"
#include "Allocator.hpp"

#include <utility>

namespace Ocean {

	Allocator::Allocator(const size_t initSize) noexcept { }

	Allocator::~Allocator() noexcept {
		OC_CORE_ASSERT(m_AllocCount == 0 && m_Used == 0, "Allocator::~Allocator Not all allocated memory has been freed!");
	}

}	// Ocean
