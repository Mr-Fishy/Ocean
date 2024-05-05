#pragma once

#include "Ocean/Memory/Allocator.hpp"
#include "Ocean/Memory/List/SingleLinkedList.hpp"

namespace Ocean {

	class FreeListAlloc : public Allocator {
	public:
		enum SearchPolicy {
			FIND_FIRST,
			FIND_BEST
		};		

		/* --- */

		FreeListAlloc(const size_t initSize, const SearchPolicy policy) noexcept;
		virtual ~FreeListAlloc() noexcept;

		virtual void Reset() override;

		virtual void* Alloc(const size_t& size, MemTypeTags tag = MemTag::UNKOWN, const size_t alignment = 8) override;
		virtual void Free(const void* ptr) override;

		virtual void ResizePool() override;

		void* GetStart() const noexcept { return m_Start; }

	private:
		struct FreeHeader {
			size_t BlockSize;
		};

		struct AllocationHeader {
			size_t BlockSize;

			char Padding;
		};

		typedef SingleLinkedList<FreeHeader>::Node Node;

		/* --- */

		void Coalescence(Node* prevNode, Node* freeNode);

		void Find(const size_t size, const size_t alignment, size_t& padding, Node*& prevNode, Node*& foundNode);
		void FindBest(const size_t size, const size_t alignment, size_t& padding, Node*& prevNode, Node*& foundNode) const;
		void FindFirst(const size_t size, const size_t alignment, size_t& padding, Node*& prevNode, Node*& foundNode) const;

		/* --- */

		void* m_Start = nullptr;

		SearchPolicy m_SearchPolicy = SearchPolicy::FIND_BEST;
		SingleLinkedList<FreeHeader> m_FreeList;
	};

}	// Ocean
