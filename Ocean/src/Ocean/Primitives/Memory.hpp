#pragma once

#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/Strings.hpp"

#include "Ocean/Primitives/Service.hpp"

// std
#include <cstddef>
#include <cstdint>

// Define this for detailed logs of where allocations and frees are completed (only when using alloc / free macros)
#define OC_DETAILED_ALLOCATIONS 1

// So that compiler doesn't give warnings about unused includes.
#ifdef OC_DETAILED_ALLOCATIONS

	#include "Ocean/Primitives/Macros.hpp"
	#include "Ocean/Primitives/Log.hpp"

#endif

#define okilo(size) (size * 1024)
#define omega(size) (size * 1024 * 1024)
#define ogiga(size) (size * 1024 * 1024 * 1024)



uintptr_t oAlignmentOffset(sizet alignOf, const void* const ptr);

uintptr_t oAlignmentAdjustment(sizet alignOf, const void* const ptr);



/**
 * @brief A struct that holds stats about memory allocations in the lifetime.
 */
struct MemoryStats {

	sizet allocatedBytes; /** @brief The total number of bytes allocated in the stat's lifetime. */
	sizet freedBytes; /** @brief The total number of bytes freed in the stat's lifetime. */

	u32 allocationCount; /** @brief The number of allocations made in the stat's lifetime. */
	u32 freeCount; /** @brief The number of frees made in the stat's lifetime. */

	/**
	 * @brief Add's data to the stat's such as the total allocatedBytes and the allocationCount. 
	 * 
	 * @param a The number of bytes to add.
	 * @return The original value.
	 */
	sizet Add(sizet a) {
		if (a) {
			this->allocatedBytes += a;
			this->allocationCount++;
		}

		return a;
	}

	/**
	 * @brief Add's data to the stat's such as the total freedBytes and the freeCount.
	 * 
	 * @param a The number of bytes to "remove".
	 * @return The original value.
	 */
	sizet Remove(sizet a) {
		if (a) {
			this->freedBytes += a;
			this->freeCount++;
		}

		return a;
	}

};	// MemoryStats



/**
 * @brief The base class of a memory allocator.
 */
class Allocator {
public:
	virtual ~Allocator() = default;

	/**
	 * @brief Allocates memory of the given size and alignment.
	 * 
	 * @param size The size in bytes to allocate.
	 * @param alignment The alignment of the allocation.
	 * @return void* 
	 */
	virtual void* Allocate(sizet size, sizet alignment = alignof(max_align_t)) = 0;

	/**
	 * @brief Deallocates memory at the given pointer.
	 * 
	 * @param ptr The pointer to the memory to deallocate.
	 */
	virtual void Deallocate(void* ptr) = 0;

};	// Allocator



/**
 * @brief The heap allocator allocates memory in as requested blocks.
 */
class HeapAllocator : public Allocator {
public:
	~HeapAllocator() override;

	/**
	 * @brief Initializes the HeapAllocator to store a maximum of the given size.
	 * 
	 * @param size The amount of memory to store in bytes.
	 */
	void Init(sizet size);
	/**
	 * @brief Clears all of the memory and shuts down the HeapAllocator.
	 */
	void Shutdown();

	/**
	 * @copydoc Allocator::Allocate()
	 */
	virtual void* Allocate(sizet size, sizet alignment) override;

	/**
	 * @copydoc Allocator::Deallocate() 
	 */
	virtual void Deallocate(void* ptr) override;

private:
	void* p_Handle = nullptr; /** @brief The tlsf handle for the memory pool. */
	void* p_Memory = nullptr; /** @brief The base memory pointer of the heap. */

	sizet m_TotalSize = 0; /** @brief The total size of the heap. */
	sizet m_AllocatedSize = 0; /** @brief The amount of memory that is allocated in the heap. */

};	// HeapAllocator



/**
 * @brief An allocator that treats the memory as a stack, that can be pushed to and popped from.
 */
class StackAllocator : public Allocator {
public:
	/**
	 * @brief Initializes the StackAllocator to store a maximum of the given size.
	 * 
	 * @param size The amount of memory to store in bytes.
	 */
	void Init(sizet size);
	/**
	 * @brief Clears all of the memory and shuts down the StackAllocator.
	 */
	void Shutdown();

	/**
	 * @copydoc Allocator::Allocate()
	 */
	virtual void* Allocate(sizet size, sizet alignment) override;

	/**
	 * @copydoc Allocator::Deallocate() 
	 */
	virtual void Deallocate(void* ptr) override;

	/**
	 * @brief Get the marker of the stack.
	 * 
	 * @return sizet
	 */
	sizet GetMarker() const;
	/**
	 * @brief Free's the 
	 * 
	 * @param marker 
	 */
	void FreeMarker(sizet marker);

	/**
	 * @brief Clear's the memory and reset's the marker.
	 */
	void Clear();

private:
	u8* p_Memory = nullptr; /** @brief The base memory pointer of the allocator. */

	sizet m_TotalSize = 0; /** @brief The total size of the allocator. */
	sizet m_AllocatedSize = 0; /** @brief The amount of memory that is allocated in the allocator. */

};	// StackAllocator



/**
 * @brief An allocator that treats the memory as a double sided stack, that can be pushed and popped from both ends.
 */
class DoubleStackAllocator : public Allocator {
public:
	/**
	 * @brief Initializes the DoubleStackAllocator to store a maximum of the given size.
	 * 
	 * @param size The amount of memory to store in bytes.
	 */
	void Init(sizet size);
	/**
	 * @brief Clears all of the memory and shuts down the DoubleStackAllocator.
	 */
	void Shutdown();

	/**
	 * @copydoc Allocator::Allocate()
	 */
	virtual void* Allocate(sizet size, sizet alignment) override;

	/**
	 * @copydoc Allocator::Deallocate() 
	 */
	virtual void Deallocate(void* ptr) override;

	/**
	 * @brief Allocates memory at the top of the stack.
	 * 
	 * @param size The size of the memory to allocate.
	 * @param alignment The alignment of the memory.
	 *
	 * @return void* 
	 */
	void* AllocateTop(sizet size, sizet alignment);
	/**
	 * @brief Allocates memory at the bottom of the stack.
	 * 
	 * @param size The size of the memory to allocate.
	 * @param alignment The alignment of the memory.
	 *
	 * @return void* 
	 */
	void* AllocateBottom(sizet size, sizet alignment);

	/**
	 * @brief Deallocates memory at the top of the stack.
	 * 
	 * @param size The amount of memory to deallocate.
	 */
	void DeallocateTop(sizet size);
	/**
	 * @brief Deallocates memory from the bottom of the stack.
	 * 
	 * @param size The amount of memory to deallocate.
	 */
	void DeallocateBottom(sizet size);

	/**
	 * @brief Get the marker at the top of the stack.
	 * 
	 * @return sizet 
	 */
	sizet GetTopMarker() const;
	/**
	 * @brief Get the marker at the bottom of the stack.
	 * 
	 * @return sizet 
	 */
	sizet GetBottomMarker() const;

	/**
	 * @brief Reset's memory from the top marker to the given marker.
	 * 
	 * @param marker The marker index to free to.
	 */
	void FreeTopMarker(sizet marker);
	/**
	 * @brief Reset's memory from the bottom marker to the given marker.
	 * 
	 * @param marker The marker index to free to.
	 */
	void FreeBottomMarker(sizet marker);

	/**
	 * @brief Clear's the top of the stack, i.e. reset's the top marker to the top of the stack.
	 */
	void ClearTop();
	/**
	 * @brief Clear's the bottom of the stack, i.e. reset's the bottom marker to the bottom of the stack.
	 */
	void ClearBottom();

private:
	u8* p_Memory = nullptr; /** @brief The base memory pointer of the allocator. */

	sizet m_TotalSize = 0; /** @brief The total size of the allocator. */
	sizet m_Top = 0; /** @brief The size distance from the top of the memory block. */
	sizet m_Bottom = 0; /** @brief The size distance from the bottom of the memory block. */

};	// DoubleStackAllocator



/**
 * @brief An allocator that treat's memory as an array, memory cannot be deallocate memory without clearing the array.
 */
class LinearAllocator : public Allocator {
public:
	/**
	 * @brief Initializes the LinearAllocator to store a maximum of the given size.
	 * 
	 * @param size The amount of memory to store in bytes.
	 */
	void Init(sizet size);
	/**
	 * @brief Clears all of the memory and shuts down the LinearAllocator.
	 */
	void Shutdown();

	/**
	 * @copydoc Allocator::Allocate()
	 */
	virtual void* Allocate(sizet size, sizet alignment) override;

	/**
	 * @copydoc Allocator::Deallocate() 
	 */
	virtual void Deallocate(void* ptr) override;

	/**
	 * @brief Clear's the allocator's memory.
	 */
	void Clear();

private:
	u8* p_Memory = nullptr; /** @brief The base memory pointer of the allocator. */

	sizet m_TotalSize = 0; /** @brief The total size of the allocator. */
	sizet m_AllocatedSize = 0; /** @brief The amount of memory that is allocated in the allocator. */

};	// LinearAllocator



/**
 * @brief Allocates memory using classic C-style malloc and free.
 */
class MallocAllocator : public Allocator {
public:
	/**
	 * @copydoc Allocator::Allocate()
	 */
	virtual void* Allocate(sizet size, sizet alignment) override;

	/**
	 * @copydoc Allocator::Deallocate() 
	 */
	virtual void Deallocate(void* ptr) override;

};	// MallocAllocator



/**
 * @brief A struct that can be used to configure the MemoryService outside of defaults.
 */
struct MemoryServiceConfig {

	sizet MaxDynamicSize = omega(16); /** @brief Default size of 16MB of dynamic memory. */

};	// MemoryServiceConfig

/**
 * @brief The primary service to interact with to get memory in Ocean.
 */
class MemoryService : public Service {
public:
	MemoryService() : m_SystemAllocator(), m_MallocAllocator() { }
	~MemoryService() = default;

	/**
	 * @brief Get's the instance of the MemoryService.
	 * 
	 * @return MemoryService& 
	 */
	static MemoryService& Instance();

	/**
	 * @brief Initializes the MemoryService and it's allocators.
	 * 
	 * @param config The MemoryServiceConfig to specify configuration options. (OPTIONAL)
	 */
	void Init(MemoryServiceConfig* config = nullptr);
	/**
	 * @brief 
	 */
	static void Shutdown();

	/**
	 * @brief Get's the system allocator of Ocean. I.e. the allocator used internally.
	 * 
	 * @return Allocator* 
	 */
	Allocator* SystemAllocator()    { return &m_SystemAllocator; }
	/**
	 * @brief Get's the system allocator of the System. Which is unmanaged by Ocean.
	 * 
	 * @return Allocator* 
	 */
	Allocator* UnmanagedAllocator() { return &m_MallocAllocator; }

	/**
	 * @brief Get's the name of the MemoryService.
	 * 
	 * @return cstring 
	 */
	inline virtual cstring GetName() const override { return "OCEAN_Memory_Service"; }

private:
	static inline MemoryService* s_Instance =  nullptr; /** @brief The MemoryService instance pointer. */

	HeapAllocator   m_SystemAllocator; /** @brief The HeapAllocator for Ocean's core allocations. */
	MallocAllocator m_MallocAllocator; /** @brief The unmanaged allocator that uses malloc and free. */

};	// MemoryService

/** @todo STL compliant allocator that routes to Ocean allocators. Example code at: https://howardhinnant.github.io/allocator_boilerplate.html */

/** @brief Macro to get the system allocator from the MemoryService. */
#define oSystemAllocator                     MemoryService::Instance().SystemAllocator()
/** @brief Macro to get the unmanaged allocator from the MemoryService. */
#define oUnmanagedAllocator                  MemoryService::Instance().UnmanagedAllocator()

#if OC_DETAILED_ALLOCATIONS && OC_VERBOSE

	/** @brief Macro to allocate a chunk of memory of the given size from the given allocator. (VERBOSE implementation). @returns void* */
	#define oalloca(size, allocator)			 ((allocator)->Allocate(size, 1)); oprintret(OCEAN_FUNCTIONLINE("Allocation Made!"))
	/** @brief Macro to allocate a chunk of memory of the given size from the given allocator. (VERBOSE implementation). @returns u8* */
	#define oallocam(size, allocator)			 (static_cast<u8*>((allocator)->Allocate(size, 1)); oprintret(OCEAN_FUNCTIONLINE("Mapped Allocation Made!"))
	/** @brief Macro to allocate an array of memory of the given type and count from the given allocator. (VERBOSE implementation). @returns type* */
	#define oallocat(type, count, allocator)	 (static_cast<type*>((allocator)->Allocate(sizeof(type) * count, alignof(type)))); oprintret(OCEAN_FUNCTIONLINE("Type Allocation Made!"))

	/** @brief Macro to allocate a chunk of memory of the given size from the given allocator with the given alignment. (VERBOSE implementation). @returns void* */
	#define oallocaa(size, allocator, alignment) ((allocator)->Allocate(size, alignment)); oprintret(OCEAN_FUNCTIONLINE("Aligned Allocation Made!"))

	/** @brief Macro to deallocate a chunk of memory from the given allocator with the given pointer. (VERBOSE implementation). */
	#define ofree(pointer, allocator)			 ((allocator)->Deallocate(pointer)); oprintret(OCEAN_FUNCTIONLINE("Memory Freed!"))

#else

	/** @brief Macro to allocate a chunk of memory of the given size from the given allocator. @returns void* */
	#define oalloca(size, allocator)			 ((allocator)->Allocate(size, 1))
	/** @brief Macro to allocate a chunk of memory of the given size from the given allocator. @returns u8* */
	#define oallocam(size, allocator)			 (static_cast<u8*>((allocator)->Allocate(size, 1))
	/** @brief Macro to allocate an array of memory of the given type and count from the given allocator. @returns type* */
	#define oallocat(type, count, allocator)	 (static_cast<type*>((allocator)->Allocate(sizeof(type) * count, alignof(type))))

	/** @brief Macro to allocate a chunk of memory of the given size from the given allocator with the given alignment. @returns void* */
	#define oallocaa(size, allocator, alignment) ((allocator)->Allocate(size, alignment))

	/** @brief Macro to deallocate a chunk of memory from the given allocator with the given pointer. */
	#define ofree(pointer, allocator)			 ((allocator)->Deallocate(pointer))

#endif
