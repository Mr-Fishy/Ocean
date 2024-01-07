#pragma once

#include "Ocean/Core/Log.hpp"

namespace Ocean {

	class MemoryManager {
	public:
		// Machine word size.
		// - 8 bytes on x64 architecture.
		// - 4 bytes on x32 architecture.
		//
		using word_t = intptr_t;

		// Allocated block of memory. Contains the object structure and the payload pointer.
		//
		struct Block {
			// Block size
			//
			size_t size;

			// Whether this block is in use
			//
			bool used;

			// Next block in the list
			//
			Block* next;

			// Payload pointer
			//
			word_t data[1];
		};

		// The heap start. Initiallized at first allocation.
		//
		static inline Block* m_HeapStart = nullptr;

		// Current top. Updated at each allocation.
		//
		static inline auto m_Top = m_HeapStart;

		// Allocates a block of memory of (at least) 'size' bytes.
		// @param size: The size to align to.
		// @returns The block data.
		//
		static word_t* alloc(size_t size) {
			size = align(size);

			auto block = requestFromOS(size);

			block->size = size;
			block->used = true;

			// Initialize heap.
			if (m_HeapStart == nullptr) {
				m_HeapStart = block;
			}

			// Chain the allocated blocks.
			if (m_Top != nullptr) {
				m_Top->next = block;
			}

			m_Top = block;

			// User payload;
			return block->data;
		}

		// Aligns the memory by the size of the machine word.
		// @param size: The size to align to.
		//
		static inline size_t align(size_t n) {
			return (n + sizeof(word_t) - 1) & ~(sizeof(word_t) - 1);
		}

		// Returns the total allocated size, reserving in addition the space for the Block structure (object header & first data word)
		// Since 'word_t data[1]' already allocates one word inside the Block structure, we can decrease it from the size request.
		// - If only one word allocates, it's fully in the Block struct.
		//
		static inline size_t allocSize(size_t size) {
			return size + sizeof(Block) - sizeof(std::declval<Block>().data);
		}

		// Returns the object header.
		//
		static Block* getHeader(word_t* data) {
			return (Block*)((char*)data + sizeof(std::declval<Block>().data) - sizeof(Block));
		}

		// Requests (mapped) memory from the OS.
		//
		static Block* requestFromOS(size_t size) {
			// Current heap break.
			auto block = (Block*)malloc(0);

			// Out Of Memory (OOM) check
			if (malloc(allocSize(size)) == (void*)-1) {
				return nullptr;
			}

			return block;
		}
	};

}	// Ocean