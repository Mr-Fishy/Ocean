#pragma once

namespace Ocean {

	class Memory {
	public:
		typedef enum MemoryTag {
			// Temporary use. Should be assigned one of the below or have a new tag created accordingly.
			// TODO: Simplify to only used memory tags later in development.
			MEMORY_TAG_UNKOWN,
			MEMORY_TAG_ARRAY,
			MEMORY_TAG_VECTOR,
			// MEMORY_TAG_DICT,
			// MEMORY_TAG_RING_QUEUE,
			// MEMORY_TAG_BST,
			MEMORY_TAG_STRING,
			MEMORY_TAG_APPLICATION,
			MEMORY_TAG_JOB,
			MEMORY_TAG_TEXTURE,
			MEMORY_TAG_MATERIAL_INSTANCE,
			MEMORY_TAG_RENDERER,
			MEMORY_TAG_RUNTIME,
			MEMORY_TAG_TRANSFORM,
			MEMORY_TAG_ENTITY,
			MEMORY_TAG_ENTITY_NODE,
			MEMORY_TAG_SCENE,

			MEMORY_TAG_MAX_TAGS
		} MemoryTag;

		static void InitMemory();
		static void ShutdownMemory();

		static void* mAllocate(uint64_t size, MemoryTag tag);
		static void mFree(void* block, uint64_t size, MemoryTag tag);

		static void* mZeroMemory(void* block, uint64_t size);

		static void* mCopyMemory(void* dest, const void* source, uint64_t size);
		static void* mSetMemory(void* dest, int32_t value, uint64_t size);

		static char* GetMemoryUsageStr();
	};

}	// Ocean
