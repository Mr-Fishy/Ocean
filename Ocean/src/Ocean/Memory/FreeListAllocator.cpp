
#include "ocpch.hpp"
#include "FreeListAllocator.hpp"

namespace Ocean {

	FreeListAlloc::FreeListAlloc(const size_t initSize, SearchPolicy policy) noexcept
		: Allocator(initSize), m_SearchPolicy(policy), m_FreeList() {
		if (m_Start != nullptr)
			free(m_Start);

		m_Start = malloc(m_Size);

		this->Reset();
	}

	FreeListAlloc::~FreeListAlloc() noexcept {
		OC_CORE_ASSERT(m_AllocCount == 0 && m_Used == 0, "FreeListAlloc::~FreeListAlloc Not all allocated memory has been freed!");

		memset(m_Start, 0, m_Size); // Clear any data in memory before releasing it.
		free(m_Start);				// Free the memory block.

		m_Start = nullptr;
	}

	void FreeListAlloc::Reset() {
		m_Used = 0;
		m_Peak = 0;
		
		Node* firstNode = static_cast<Node*>(m_Start);
		firstNode->Data.BlockSize = m_Size;
		firstNode->Next = nullptr;

		m_FreeList.m_Head = nullptr;
		m_FreeList.Insert(nullptr, firstNode);
	}

	void* FreeListAlloc::Alloc(const size_t& size, MemTypeTags tag, const size_t alignment) {
		OC_CORE_ASSERT(size > sizeof(Node) && alignment >= 8, "FreeListAlloc::Alloc Has been given an invalid size or alignment value!");

		const size_t allocHeaderSize = sizeof(AllocationHeader);
		const size_t freeHeaderSize = sizeof(FreeHeader);

		size_t padding;
		Node* affected, * previous;

		this->Find(size, alignment, padding, previous, affected);

		OC_CORE_ASSERT(affected != nullptr, "FreeListAlloc::Alloc() Not enough memory!"); // Can have resize here

		const size_t alignmentPadding = padding - allocHeaderSize;
		const size_t reqSize = size + padding;

		const size_t rest = affected->Data.BlockSize - reqSize;

		if (rest > 0) {
			Node* newFreeNode = reinterpret_cast<Node*>(reinterpret_cast<size_t>(affected) + reqSize);
			newFreeNode->Data.BlockSize = rest;

			m_FreeList.Insert(affected, newFreeNode);
		}
		m_FreeList.Remove(previous, affected);

		const size_t header = reinterpret_cast<size_t>(affected) + alignmentPadding;
		const size_t data = header + allocHeaderSize;
		((AllocationHeader*)header)->BlockSize = reqSize;
		((AllocationHeader*)header)->Padding = static_cast<char>(alignmentPadding);

		m_Used += reqSize;
		m_UsedList[tag] += size + padding;
		m_Peak = std::max(m_Peak, m_Used);

		return reinterpret_cast<void*>(data);
	}

	void FreeListAlloc::Free(const void* ptr) {
		const size_t current = reinterpret_cast<size_t>(ptr);
		const size_t header = current - sizeof(AllocationHeader);
		const AllocationHeader* allocationHeader{ (AllocationHeader*)header };

		Node* freeNode = reinterpret_cast<Node*>(header);
		freeNode->Data.BlockSize = allocationHeader->BlockSize + allocationHeader->Padding;
		freeNode->Next = nullptr;

		Node* it = m_FreeList.m_Head;
		Node* prev = nullptr;
		while (it != nullptr) {
			if (ptr < it) {
				m_FreeList.Insert(prev, freeNode);
				
				break;
			}

			prev = it;
			it = it->Next;
		}

		m_Used -= freeNode->Data.BlockSize;

		Coalescence(prev, freeNode);
	}

	void FreeListAlloc::ResizePool() {
		OC_CORE_ASSERT(false, "LinearAlloc::ResizePool() Is not implemented!");
	}

	void FreeListAlloc::Coalescence(Node* prevNode, Node* freeNode) {
		if (freeNode->Next != nullptr && reinterpret_cast<size_t>(freeNode) + freeNode->Data.BlockSize == reinterpret_cast<size_t>(freeNode->Next)) {
			freeNode->Data.BlockSize += freeNode->Next->Data.BlockSize;

			m_FreeList.Remove(freeNode, freeNode->Next);
		}

		if (prevNode != nullptr && reinterpret_cast<size_t>(prevNode) + prevNode->Data.BlockSize == reinterpret_cast<size_t>(prevNode)) {
			prevNode->Data.BlockSize += freeNode->Data.BlockSize;

			m_FreeList.Remove(prevNode, freeNode);
		}
	}

	void FreeListAlloc::Find(const size_t size, const size_t alignment, size_t& padding, Node*& prevNode, Node*& foundNode) {
		switch (m_SearchPolicy) {
			case FreeListAlloc::FIND_FIRST:
				FindFirst(size, alignment, padding, prevNode, foundNode);
				break;

			case FreeListAlloc::FIND_BEST:
				FindBest(size, alignment, padding, prevNode, foundNode);
				break;
		}
	}

	void FreeListAlloc::FindBest(const size_t size, const size_t alignment, size_t& padding, Node*& prevNode, Node*& foundNode) const {
		size_t smallestDiff = std::numeric_limits<size_t>::max();
		Node* it = m_FreeList.m_Head, * prev = nullptr;
		Node* best = nullptr;

		while (it != nullptr) {
			padding = Internal::Mem::CalculatePaddingWithHeader(reinterpret_cast<size_t>(it), alignment, sizeof(AllocationHeader));
			const size_t reqSpace = size + padding;

			if (it->Data.BlockSize >= reqSpace && (it->Data.BlockSize - reqSpace < smallestDiff))
				best = it;

			prev = it;
			it = it->Next;
		}

		prevNode = prev;
		foundNode = best;
	}

	void FreeListAlloc::FindFirst(const size_t size, const size_t alignment, size_t& padding, Node*& prevNode, Node*& foundNode) const {
		Node* it = m_FreeList.m_Head, * prev = nullptr;

		while (it != nullptr) {
			padding = Internal::Mem::CalculatePaddingWithHeader(reinterpret_cast<size_t>(it), alignment, sizeof(AllocationHeader));
			const size_t reqSpace = size + padding;

			if (it->Data.BlockSize >= reqSpace)
				break;

			prev = it;
			it = it->Next;
		}

		prevNode = prev;
		foundNode = it;
	}

}	// Ocean
