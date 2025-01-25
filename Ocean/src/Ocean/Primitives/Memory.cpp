#include "Memory.hpp"

#include "Ocean/Primitives/Assert.hpp"

// libs
#include <tlsf.h>

// std
#include <stdlib.h>
#include <cstdint>
#include <cstring>

// Memory Methods

// The following offset and adjustment implementations are adapted from:
// https://screwjankgames.github.io/engine%20programming/2020/09/24/writing-your-own-memory-allocators.html

uintptr_t oAlignmentOffset(sizet alignOf, const void* const ptr) {
	// Note: m % n & = m & (n - 1) if n is a power of two.
	// Alignment requirements are always a power of two.

	return reinterpret_cast<uintptr_t>(ptr) & (alignOf - 1);
}

uintptr_t oAlignmentAdjustment(sizet alignOf, const void* const ptr) {
	auto offset = oAlignmentOffset(alignOf, ptr);

	// If the address is already aligned we don't need any adjustment.

	if (offset == 0)
		return 0;

	return alignOf - offset;
}

// Walker Methods

#ifdef OC_DETAILED_ALLOCATIONS
	static void ExitWalker(void* ptr, sizet size, i32 used, OC_UNUSED void* user) {
		if (used) {
			oprint("Found active allocation %p, %llu\n", ptr, size);
		}
	}

#endif

// Heap Allocator

#ifdef OC_DETAILED_ALLOCATIONS

	static MemoryStats s_Stats{ 0, 0, 0, 0 };

#endif

HeapAllocator::~HeapAllocator() { }

void HeapAllocator::Init(sizet size) {
	this->p_Memory = malloc(size);
	this->m_TotalSize = size;
	this->m_AllocatedSize = 0;

	this->p_Handle = tlsf_create_with_pool(this->p_Memory, size);
}

void HeapAllocator::Shutdown() {
#ifdef OC_DETAILED_ALLOCATIONS
	if (s_Stats.allocatedBytes != s_Stats.freedBytes)
		oprint(CONSOLE_TEXT_RED("Allocations still present. Check your code!\n"));

	void* pool = tlsf_get_pool(this->p_Handle);
	tlsf_walk_pool(pool, ExitWalker, static_cast<void*>(&s_Stats));

	oprint(CONSOLE_TEXT_CYAN("Memory Heap Stats:\n"));
	oprint(CONSOLE_TEXT_CYAN("\tAllocated Bytes: %i\n"), s_Stats.allocatedBytes);
	oprint(CONSOLE_TEXT_CYAN("\tFreed Bytes: %i\n"), s_Stats.freedBytes);
	oprint(CONSOLE_TEXT_CYAN("\tTotal Allocations: %i\n"), s_Stats.allocationCount);
	oprint(CONSOLE_TEXT_CYAN("\tTotal Free's: %i\n"), s_Stats.freeCount);

#endif

	tlsf_destroy(this->p_Handle);

	free(this->p_Memory);
	this->p_Memory = this->p_Handle = nullptr;
	this->m_TotalSize = this->m_AllocatedSize = 0;
}

void* HeapAllocator::Allocate(sizet size, OC_UNUSED sizet alignment) {
#ifdef OC_DETAILED_ALLOCATIONS

	void* allocatedMemory = alignment == 1 ? tlsf_malloc(this->p_Handle, size) : tlsf_memalign(this->p_Handle, alignment, size);
	this->m_AllocatedSize += s_Stats.Add(tlsf_block_size(allocatedMemory));

	return allocatedMemory;

#else

	return tlsf_malloc(this->p_Handle, size);

#endif
}

void HeapAllocator::Deallocate(void* ptr) {
#ifdef OC_DETAILED_ALLOCATIONS

	this->m_AllocatedSize -= s_Stats.Remove(tlsf_block_size(ptr));

	tlsf_free(this->p_Handle, ptr);

#else

	tlsf_free(this->p_Handle, ptr);

#endif
}

// Stack Allocator

void StackAllocator::Init(sizet size) {
	this->p_Memory = static_cast<u8*>(malloc(size));
	this->m_AllocatedSize = 0;
	this->m_TotalSize = size;
}

void StackAllocator::Shutdown() {
	free(this->p_Memory);
}

void* StackAllocator::Allocate(sizet size, sizet alignment) {
	OASSERT(size > 0);

	const sizet newStart = oAlignmentAdjustment(alignment, this->p_Memory + this->m_AllocatedSize);
	OASSERT(newStart > this->m_TotalSize);

	const sizet newSize = newStart + size;
	if (newSize > this->m_TotalSize) {
		OASSERTM(false, "MEMORY OVERFLOW |: Stack Allocator");
		return nullptr;
	}

	this->m_AllocatedSize = newSize;
	return this->p_Memory + newStart;
}

void StackAllocator::Deallocate(void* ptr) {
	OASSERT(ptr >= this->p_Memory);
	OASSERTM(ptr < this->p_Memory + this->m_TotalSize, "Out of bound free on Stack Allocator. Attempting to free %p, %llu after beginning of buffer (memory %p, size %llu, allocated %llu).", static_cast<u8*>(ptr), static_cast<u8*>(ptr) - p_Memory, p_Memory, m_TotalSize, m_AllocatedSize);
	OASSERTM(ptr < this->p_Memory + this->m_AllocatedSize, "Out of allocated bound free on Stack Allocator. Attempting to free %p, %llu after beginning of buffer (memory %p, size %llu, allocated %llu).", static_cast<u8*>(ptr), static_cast<u8*>(ptr) - p_Memory, p_Memory, m_TotalSize, m_AllocatedSize);

	this->m_AllocatedSize = static_cast<u8*>(ptr) - this->p_Memory;
}

sizet StackAllocator::GetMarker() const {
	return this->m_AllocatedSize;
}

void StackAllocator::FreeMarker(sizet marker) {
	if (marker - this->m_AllocatedSize > 0)
		this->m_AllocatedSize = marker;
}

void StackAllocator::Clear() {
	memset(this->p_Memory, 0, this->m_TotalSize);
	this->m_AllocatedSize = 0;
}

// Double Stack Allocator

void DoubleStackAllocator::Init(sizet size) {
	this->p_Memory = static_cast<u8*>(malloc(size));
	this->m_TotalSize = this->m_Top = size;
	this->m_Bottom = 0;
}

void DoubleStackAllocator::Shutdown() {
	free(this->p_Memory);
}

void* DoubleStackAllocator::Allocate(OC_UNUSED sizet size, OC_UNUSED sizet alignment) {
	OASSERT(false);
	return nullptr;
}

void DoubleStackAllocator::Deallocate(OC_UNUSED void* ptr) {
	OASSERT(false);
}

void* DoubleStackAllocator::AllocateTop(sizet size, sizet alignment) {
	OASSERT(size > 0);

	const sizet newStart = oAlignmentAdjustment(alignment, this->p_Memory + this->m_Top);
	if (newStart <= this->m_Bottom) {
		OASSERTM(false, "MEMORY OVERFLOW CROSSING |: Double Stack Allocator");
		return nullptr;
	}

#ifdef OC_DETAILED_ALLOCATIONS

	s_Stats.Add(size);

#endif

	this->m_Top = newStart;
	return this->p_Memory + newStart;
}

void* DoubleStackAllocator::AllocateBottom(sizet size, sizet alignment) {
	OASSERT(size > 0);

	const sizet newStart = oAlignmentAdjustment(alignment, this->p_Memory + this->m_Bottom);
	const sizet newSize = newStart + size;

	if (newSize >= m_Top) {
		OASSERTM(false, "MEMORY OVERFLOW CROSSING |: Double Stack Allocator");
		return nullptr;
	}

#ifdef OC_DETAILED_ALLOCATIONS

	s_Stats.Add(size);

#endif

	this->m_Bottom = newSize;
	return this->p_Memory + newStart;
}

void DoubleStackAllocator::DeallocateTop(sizet size) {
	if (size > this->m_TotalSize - this->m_Top)
		this->m_Top = this->m_TotalSize;
	else
		this->m_Top += size;

#ifdef OC_DETAILED_ALLOCATIONS

	s_Stats.Remove(size);

#endif
}

void DoubleStackAllocator::DeallocateBottom(sizet size) {
	if (size > this->m_Bottom)
		this->m_Bottom = 0;
	else
		this->m_Bottom -= size;

#ifdef OC_DETAILED_ALLOCATIONS

	s_Stats.Remove(size);

#endif
}

sizet DoubleStackAllocator::GetTopMarker() const {
	return this->m_Top;
}

sizet DoubleStackAllocator::GetBottomMarker() const {
	return this->m_Bottom;
}

void DoubleStackAllocator::FreeTopMarker(sizet marker) {
	if (marker > this->m_Top && marker < this->m_TotalSize)
		this->m_Top = marker;
}

void DoubleStackAllocator::FreeBottomMarker(sizet marker) {
	if (marker < this->m_Bottom)
		this->m_Bottom = marker;
}

void DoubleStackAllocator::ClearTop() {
	this->m_Top = this->m_TotalSize;
}

void DoubleStackAllocator::ClearBottom() {
	this->m_Bottom = 0;
}

// Linear Allocator

void LinearAllocator::Init(sizet size) {
	this->p_Memory = static_cast<u8*>(malloc(size));
	this->m_TotalSize = size;
	this->m_AllocatedSize = 0;
}

void LinearAllocator::Shutdown() {
	free(this->p_Memory);
}

void* LinearAllocator::Allocate(sizet size, sizet alignment) {
	OASSERT(size > 0);

	const sizet newStart = oAlignmentAdjustment(alignment, this->p_Memory + this->m_AllocatedSize);

	OASSERT(newStart < this->m_TotalSize);

	const sizet newSize = newStart + size;

	if (newSize > this->m_TotalSize) {
		OASSERTM(false, "MEMORY OVERFLOW |: Linear Allocator");
		return nullptr;
	}

#ifdef OC_DETAILED_ALLOCATIONS

	s_Stats.Add(size);

#endif

	this->m_AllocatedSize = newSize;
	return this->p_Memory + newStart;
}

void LinearAllocator::Deallocate(OC_UNUSED void* ptr) {
	oprint("Linear Allocator Deallocate called when it does not do anything!");
	// This allocator does not allocate on a per-pointer base.
}

void LinearAllocator::Clear() {
#ifdef OC_DETAILED_ALLOCATIONS

	s_Stats.Remove(this->m_AllocatedSize);

#endif

	this->m_AllocatedSize = 0;
}

// Malloc Allocator

void* MallocAllocator::Allocate(sizet size, OC_UNUSED sizet alignment) {
#ifdef OC_DETAILED_ALLOCATIONS

	// Since we can't track the amount of memory freed, don't add to the allocation amount (at least not yet).
	s_Stats.Add(0);

#endif

	return malloc(size);
}

void MallocAllocator::Deallocate(void* ptr) {
#ifdef OC_DETAILED_ALLOCATIONS

	// Since we can't track the amount of memory freed, don't add to the deallocation amount (at least not yet).
	s_Stats.Remove(0);

#endif

	free(ptr);
}

// Memory Service

static sizet s_Size = omega(32) + tlsf_size() + 8;

MemoryService& MemoryService::Instance() {
	if (!s_Instance)
		s_Instance = new MemoryService();

	return *s_Instance;
}

void MemoryService::Init(MemoryServiceConfig* config) {
	m_SystemAllocator.Init(config ? config->MaxDynamicSize : s_Size);
}

void MemoryService::Shutdown() {
	Instance().m_SystemAllocator.Shutdown();

	delete &Instance();
}