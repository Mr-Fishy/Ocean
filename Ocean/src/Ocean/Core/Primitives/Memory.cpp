#include "Memory.hpp"

#include "Ocean/Core/Primitives/MemoryUtils.hpp"
#include "Ocean/Core/Primitives/Assert.hpp"

#include "tlsf.h"

// std
#include <stdlib.h>
#include <memory>

// Define the following to enable StackWalker to heavy memory profile
// #define OCEAN_MEMORY_STACK

#define HEAP_ALLOCATOR_STATS

#if defined(OCEAN_MEMORY_STACK)

	#include "StackWalker/StackWalker.h"

#endif

namespace Ocean {

	// Walker Methods

	static void ExitWalker(void* ptr, sizet size, i32 used, void* user) {
		MemoryStats* stats = static_cast<MemoryStats*>(user);

		stats->Add(used ? size : 0);

		if (used)
			oprint("Found active allocation %p, %llu\n", ptr, size);
	}

	// Memory Service

	static MemoryService s_MemoryService;

	static sizet s_Size = omega(32) + tlsf_size() + 8;

	MemoryService* MemoryService::Instance() {
		return &s_MemoryService;
	}

	void MemoryService::Init(void* config) {
		MemoryServiceConfig* memoryConfig = static_cast<MemoryServiceConfig*>(config);
		m_SystemAllocator.Init(memoryConfig ? memoryConfig->MaxDynamicSize : s_Size);
	}

	void MemoryService::Shutdown() {
		m_SystemAllocator.Shutdown();
	}

	void MemoryService::Test() {
		// TODO
	}

	// Memory Methods

	void mem_copy(void* dst, void* src, sizet size) {
		memcpy(dst, src, size);
	}

	sizet mem_align(sizet size, sizet alignment) {
		const sizet alignmentMask = alignment - 1;

		return (size + alignmentMask) & ~alignmentMask;
	}

	// Heap Allocator

	HeapAllocator::~HeapAllocator() { }

	// TODO: Finish implementing Memory Stats so that it is accurate.
	void HeapAllocator::Init(sizet size) {
		p_Memory = malloc(size);
		m_TotalSize = size;
		m_AllocatedSize = 0;

		p_Handle = tlsf_create_with_pool(p_Memory, size);
	}

	void HeapAllocator::Shutdown() {
		MemoryStats stats{ m_AllocatedSize, m_TotalSize };
		void* pool = tlsf_get_pool(p_Handle);
		tlsf_walk_pool(pool, ExitWalker, (void*)&stats);

		OASSERTM(stats.AllocatedBytes == 0, "Allocations still present. Check your code!");

		tlsf_destroy(p_Handle);

		free(p_Memory);
		p_Memory = p_Handle = nullptr;
		m_TotalSize = m_AllocatedSize = 0;
	}

	void* HeapAllocator::Allocate(sizet size) {
		return Allocate(size, 1);
	}

#if defined (OCEAN_MEMORY_STACK)

	class OceanStackWalker : public StackWalker {
	public:
		OceanStackWalker() : StackWalker() { }

	protected:
		virtual void OnOutput(LPCSTR szText) {
			oprint("\nStack: \n%s\n", szText);

			StackWalker::OnOutput(szText);
		}

	};	// OceanStackWalker

	void* HeapAllocator::Allocate(sizet size, sizet alignment) {
		void* allocatedMemory = oMalloc(p_Handle, size);

		oprint("Mem: %p, size %llu\n", allocatedMemory, size);

		return allocatedMemory;
	}

#else

	void* HeapAllocator::Allocate(sizet size, sizet alignment) {
	#if defined (HEAP_ALLOCATOR_STATS)

		void* allocatedMemory = alignment == 1 ? tlsf_malloc(p_Handle, size) : tlsf_memalign(p_Handle, alignment, size);
		m_AllocatedSize += tlsf_block_size(allocatedMemory);

		return allocatedMemory;

	#else

		return tlsf_malloc(p_Handle, size);

	#endif
	}

#endif

	void* HeapAllocator::Allocate(sizet size, sizet alignment, cstring file, i32 line) {
		return Allocate(size, alignment);
	}

	void HeapAllocator::Deallocate(void* ptr) {
	#if defined (HEAP_ALLOCATOR_STATS)

		m_AllocatedSize -= tlsf_block_size(ptr);

		tlsf_free(p_Handle, ptr);

	#else

		tlsf_free(p_Handle, ptr);

	#endif
	}

	// Stack Allocator

	void StackAllocator::Init(sizet size) {
		p_Memory = (u8*)malloc(size);
		m_AllocatedSize = 0;
		m_TotalSize = size;
	}

	void StackAllocator::Shutdown() {
		free(p_Memory);
	}

	void* StackAllocator::Allocate(sizet size) {
		return Allocate(size, 1);
	}

	void* StackAllocator::Allocate(sizet size, sizet alignment) {
		OASSERT(size > 0);

		const sizet newStart = mem_align(m_AllocatedSize, alignment);
		OASSERT(newStart > m_TotalSize);

		const sizet newSize = newStart + size;
		if (newSize > m_TotalSize) {
			OASSERTM(false, "MEMORY OVERFLOW |: Stack Allocator");
			return nullptr;
		}

		m_AllocatedSize = newSize;
		return p_Memory + newStart;
	}

	void* StackAllocator::Allocate(sizet size, sizet alignment, cstring file, i32 line) {
		return Allocate(size, alignment);
	}

	void StackAllocator::Deallocate(void* ptr) {
		OASSERT(ptr >= p_Memory);
		OASSERTM(ptr < p_Memory + m_TotalSize, "Out of bound free on Stack Allocator. Attempting to free %p, %llu after beginning of buffer (memory %p, size %llu, allocated %llu).", (u8*)ptr, (u8*)ptr - p_Memory, p_Memory, m_TotalSize, m_AllocatedSize);
		OASSERTM(ptr < p_Memory + m_AllocatedSize, "Out of allocated bound free on Stack Allocator. Attempting to free %p, %llu after beginning of buffer (memory %p, size %llu, allocated %llu).", (u8*)ptr, (u8*)ptr - p_Memory, p_Memory, m_TotalSize, m_AllocatedSize);

		m_AllocatedSize = (u8*)ptr - p_Memory;
	}

	sizet StackAllocator::GetMarker() const {
		return m_AllocatedSize;
	}

	void StackAllocator::FreeMarker(sizet marker) {
		if (marker - m_AllocatedSize > 0)
			m_AllocatedSize = marker;
	}

	void StackAllocator::Clear() {
		memset(p_Memory, 0, m_TotalSize);
		m_AllocatedSize = 0;
	}

	// Double Stack Allocator

	void DoubleStackAllocator::Init(sizet size) {
		p_Memory = (u8*)malloc(size);
		m_TotalSize = m_Top = size;
		m_Bottom = 0;
	}

	void DoubleStackAllocator::Shutdown() {
		free(p_Memory);
	}

	void* DoubleStackAllocator::Allocate(sizet size) {
		return Allocate(size, 1);
	}

	void* DoubleStackAllocator::Allocate(sizet size, sizet alignment) {
		OASSERT(false);
		return nullptr;
	}

	void* DoubleStackAllocator::Allocate(sizet size, sizet alignment, cstring file, i32 line) {
		OASSERT(false);
		return nullptr;
	}

	void DoubleStackAllocator::Deallocate(void* ptr) {
		OASSERT(false);
	}

	void* DoubleStackAllocator::AllocateTop(sizet size, sizet alignment) {
		OASSERT(size > 0);

		const sizet newStart = mem_align(m_Top - size, alignment);
		if (newStart <= m_Bottom) {
			OASSERTM(false, "MEMORY OVERFLOW CROSSING |: Double Stack Allocator");
			return nullptr;
		}

		m_Top = newStart;
		return p_Memory + newStart;
	}

	void* DoubleStackAllocator::AllocateBottom(sizet size, sizet alignment) {
		OASSERT(size > 0);

		const sizet newStart = mem_align(m_Bottom, alignment);
		const sizet newSize = newStart + size;

		if (newSize >= m_Top) {
			OASSERTM(false, "MEMORY OVERFLOW CROSSING |: Double Stack Allocator");
			return nullptr;
		}

		m_Bottom = newSize;
		return p_Memory + newStart;
	}

	void DoubleStackAllocator::DeallocateTop(sizet size) {
		if (size > m_TotalSize - m_Top)
			m_Top = m_TotalSize;
		else
			m_Top += size;
	}

	void DoubleStackAllocator::DeallocateBottom(sizet size) {
		if (size > m_Bottom)
			m_Bottom = 0;
		else
			m_Bottom -= size;
	}

	sizet DoubleStackAllocator::GetTopMarker() const {
		return m_Top;
	}

	sizet DoubleStackAllocator::GetBottomMarker() const {
		return m_Bottom;
	}

	void DoubleStackAllocator::FreeTopMarker(sizet marker) {
		if (marker > m_Top && marker < m_TotalSize)
			m_Top = marker;
	}

	void DoubleStackAllocator::FreeBottomMarker(sizet marker) {
		if (marker < m_Bottom)
			m_Bottom = marker;
	}

	void DoubleStackAllocator::ClearTop() {
		m_Top = m_TotalSize;
	}

	void DoubleStackAllocator::ClearBottom() {
		m_Bottom = 0;
	}

	void LinearAllocator::Init(sizet size) {
		p_Memory = (u8*)malloc(size);
		m_TotalSize = size;
		m_AllocatedSize = 0;
	}

	void LinearAllocator::Shutdown() {
		free(p_Memory);
	}

	void* LinearAllocator::Allocate(sizet size) {
		return Allocate(size, 1);
	}

	void* LinearAllocator::Allocate(sizet size, sizet alignment) {
		OASSERT(size > 0);

		const sizet newStart = mem_align(m_AllocatedSize, alignment);

		OASSERT(newStart < m_TotalSize);

		const sizet newSize = newStart + size;

		if (newSize > m_TotalSize) {
			OASSERTM(false, "MEMORY OVERFLOW |: Linear Allocator");
			return nullptr;
		}

		m_AllocatedSize = newSize;
		return p_Memory + newStart;
	}

	void* LinearAllocator::Allocate(sizet size, sizet alignment, cstring file, i32 line) {
		return Allocate(size, alignment);
	}

	void LinearAllocator::Deallocate(void* ptr) {
		oprint("Linear Allocator Deallocate called when it does not do anything!");
		// This allocator does not allocate on a per-pointer base.
	}

	void LinearAllocator::Clear() {
		memset(p_Memory, 0, m_TotalSize);
		m_AllocatedSize = 0;
	}

	void* MallocAllocator::Allocate(sizet size) {
		return malloc(size);
	}

	void* MallocAllocator::Allocate(sizet size, sizet alignment) {
		return malloc(size);
	}

	void* MallocAllocator::Allocate(sizet size, sizet alignment, cstring file, i32 line) {
		return malloc(size);
	}

	void MallocAllocator::Deallocate(void* ptr) {
		free(ptr);
	}

}	// Ocean
