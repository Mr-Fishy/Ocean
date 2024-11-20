#pragma once

#include "Ocean/Core/Types/Integers.hpp"
#include "Ocean/Core/Types/Strings.hpp"

#include "Ocean/Core/Primitives/Service.hpp"
#include "Ocean/Core/Primitives/Macros.hpp"
#include "Ocean/Core/Primitives/Log.hpp"
#include <cstddef>

// Define this for detailed logs of where allocations and frees are completed (only when using alloc / free macros)
// #define DETAILED_ALLOCATIONS 1

namespace Ocean {

	#define okilo(size) (size * 1024)
	#define omega(size) (size * 1024 * 1024)
	#define ogiga(size) (size * 1024 * 1024 * 1024)

	// Memory Methods

	/**
	 * @brief Copy memory from a source to a destination.
	 * @param dst - The destination pointer.
	 * @param src - The source pointer.
	 * @param size - The size of the data.
	 */
	void mem_copy(void* dst, void* src, sizet size);

	/**
	 * @brief Calculate the aligned memory size.
	 * @param size 
	 * @param alignment 
	 * @return 
	 */
	sizet mem_align(sizet size, sizet alignment);

	// Memory Structs

	/**
	 * @brief 
	 */
	struct MemoryStats {

		sizet AllocatedBytes;
		sizet FreedBytes;

		u32 AllocationCount;
		u32 FreeCount;

		sizet Add(sizet a) {
			if (a) {
				AllocatedBytes += a;
				AllocationCount++;
			}

			return a;
		}

		sizet Remove(sizet a) {
			if (a) {
				FreedBytes += a;
				FreeCount++;
			}

			return a;
		}

	};	// MemoryStats

	// Memory Allocators

	/**
	 * @brief The base class of a memory allocator.
	 */
	class Allocator {
	public:
		virtual ~Allocator() = default;

		virtual void* Allocate(sizet size) = 0;
		virtual void* Allocate(sizet size, sizet alignment) = 0;
		virtual void* Allocate(sizet size, sizet alignment, cstring file, i32 line) = 0;

		virtual void Deallocate(void* ptr) = 0;

	};	// Allocator

	class HeapAllocator : public Allocator {
	public:
		~HeapAllocator() override;

		void Init(sizet size);
		void Shutdown();

		virtual void* Allocate(sizet size) override;
		virtual void* Allocate(sizet size, sizet alignment) override;
		virtual void* Allocate(sizet size, sizet alignment, cstring file, i32 line) override;

		virtual void Deallocate(void* ptr) override;

	private:
		void* p_Handle = nullptr;
		void* p_Memory = nullptr;

		sizet m_TotalSize = 0;
		sizet m_AllocatedSize = 0;

	};	// HeapAllocator

	class StackAllocator : public Allocator {
	public:
		void Init(sizet size);
		void Shutdown();

		virtual void* Allocate(sizet size) override;
		virtual void* Allocate(sizet size, sizet alignment) override;
		virtual void* Allocate(sizet size, sizet alignment, cstring file, i32 line) override;

		virtual void Deallocate(void* ptr) override;

		sizet GetMarker() const;
		void FreeMarker(sizet marker);

		void Clear();

	private:
		u8* p_Memory = nullptr;

		sizet m_TotalSize = 0;
		sizet m_AllocatedSize = 0;

	};	// StackAllocator

	class DoubleStackAllocator : public Allocator {
	public:
		void Init(sizet size);
		void Shutdown();

		virtual void* Allocate(sizet size) override;
		virtual void* Allocate(sizet size, sizet alignment) override;
		virtual void* Allocate(sizet size, sizet alignment, cstring file, i32 line) override;

		virtual void Deallocate(void* ptr) override;

		void* AllocateTop(sizet size, sizet alignment);
		void* AllocateBottom(sizet size, sizet alignment);

		void DeallocateTop(sizet size);
		void DeallocateBottom(sizet size);

		sizet GetTopMarker() const;
		sizet GetBottomMarker() const;

		void FreeTopMarker(sizet marker);
		void FreeBottomMarker(sizet marker);

		void ClearTop();
		void ClearBottom();

	private:
		u8* p_Memory = nullptr;

		sizet m_TotalSize = 0;
		sizet m_Top = 0;
		sizet m_Bottom = 0;

	};	// DoubleStackAllocator

	class LinearAllocator : public Allocator {
	public:
		void Init(sizet size);
		void Shutdown();

		virtual void* Allocate(sizet size) override;
		virtual void* Allocate(sizet size, sizet alignment) override;
		virtual void* Allocate(sizet size, sizet alignment, cstring file, i32 line) override;

		virtual void Deallocate(void* ptr) override;

		void Clear();

	private:
		u8* p_Memory = nullptr;

		sizet m_TotalSize = 0;
		sizet m_AllocatedSize = 0;

	};	// LinearAllocator

	class MallocAllocator : public Allocator {
	public:
		virtual void* Allocate(sizet size) override;
		virtual void* Allocate(sizet size, sizet alignment) override;
		virtual void* Allocate(sizet size, sizet alignment, cstring file, i32 line) override;

		virtual void Deallocate(void* ptr) override;

	};	// MallocAllocator

	// Memory Service

	struct MemoryServiceConfig {

		sizet MaxDynamicSize = omega(16); // Default size of 16MB of dynamic memory.

	};	// MemoryServiceConfig

	class MemoryService : public Service {
	public:
		MemoryService() : m_ScratchAllocator(), m_SystemAllocator() { }
		~MemoryService() = default;

		static MemoryService& Instance();

		void Init(MemoryServiceConfig* config = nullptr);
		static void Shutdown();

		static cstring Name() { return "OCEAN_Memory_Service"; }

		Allocator* ScratchAllocator() { return &m_ScratchAllocator; }
		Allocator* SystemAllocator()  { return &m_SystemAllocator; }

	private:
		static inline MemoryService* s_Instance =  nullptr;

		LinearAllocator m_ScratchAllocator;
		HeapAllocator   m_SystemAllocator;

	};	// MemoryService

	// Macro Helpers

	#define oalloca(size, allocator)			 ((allocator)->Allocate(size, 1, __FILE__, __LINE__))
	#define oallocam(size, allocator)			 (static_cast<u8*>((allocator)->Allocate(size, 1, __FILE__, __LINE__)))
	#define oallocat(type, count, allocator)	 (static_cast<type*>((allocator)->Allocate(sizeof(type) * count, alignof(type), __FILE__, __LINE__)))

	#define oallocaa(size, allocator, alignment) ((allocator)->Allocate(size, alignment, __FILE__, __LINE__))

	#define ofree(pointer, allocator)			 ((allocator)->Deallocate(pointer))

}	// Ocean
