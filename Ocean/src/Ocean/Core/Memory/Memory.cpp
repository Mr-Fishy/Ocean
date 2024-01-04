
#include "ocpch.hpp"
#include "Memory.hpp"

#include "Ocean/Core/Window.hpp"

namespace Ocean {

	struct MemoryStats {
		uint64_t TotalAllocated;
		uint64_t TaggedAllocations[Memory::MEMORY_TAG_MAX_TAGS];
	};

	static const char* MemoryTagStrings[Memory::MEMORY_TAG_MAX_TAGS] = {
		"UNKNOWN    ",
		"ARRAY      ",
		"VECTOR     ",
		// "DICT       ",
		// "RING_QUEUE ",
		// "BST        ",
		"STRING     ",
		"APPLICATION",
		"JOB        ",
		"TEXTURE    ",
		"MAT_INST   ",
		"RENDERER   ",
		"RUNTIME    ",
		"TRANSFORM  ",
		"ENTITY     ",
		"ENTITY_NODE",
		"SCENE      ",
	};

	static struct MemoryStats stats;

	void Memory::InitMemory() {
		Window::wZeroMemory(&stats, sizeof(stats));
	}

	void Memory::ShutdownMemory() {}

	void* Memory::mAllocate(uint64_t size, MemoryTag tag) {
		OC_CORE_ASSERT(tag != MEMORY_TAG_UNKOWN, "mAllocate called using MEMORY_TAG_UNKOWN. Re-class this allocation.");

		stats.TotalAllocated += size;
		stats.TaggedAllocations[tag] += size;

		// TODO: Memory Alignment
		void* block = Window::wAllocate(size, false);
		Window::wZeroMemory(block, size);
		
		return block;
	}

	void Memory::mFree(void* block, uint64_t size, MemoryTag tag) {
		OC_CORE_ASSERT(tag != MEMORY_TAG_UNKOWN, "mAllocate called using MEMORY_TAG_UNKOWN. Re-class this allocation.");

		stats.TotalAllocated -= size;
		stats.TaggedAllocations[tag] -= size;

		// TODO: Memory Alignment
		Window::wFree(block, false);
	}

	void* Memory::mZeroMemory(void* block, uint64_t size) {
		return Window::wZeroMemory(block, size);
	}

	void* Memory::mCopyMemory(void* dest, const void* source, uint64_t size) {
		return Window::wCopyMemory(dest, source, size);
	}

	void* Memory::mSetMemory(void* dest, int32_t value, uint64_t size) {
		return Window::wSetMemory(dest, value, size);
	}

	char* Memory::GetMemoryUsageStr() {
		const uint64_t GB = 1024 * 1024 * 1024;
		const uint64_t MB = 1024 * 1024;
		const uint64_t KB = 1024;

		char buffer[8000] = "System memory use (tagged):\n";
		uint64_t offset = strlen(buffer);

		for (uint32_t i = 0; i < MEMORY_TAG_MAX_TAGS; ++i) {
			char unit[4] = "Xib";
			float amount = 1.0f;

			if (stats.TaggedAllocations[i] >= GB) {
				unit[0] = 'G';
				amount = stats.TaggedAllocations[i] / (float)GB;
			}
			else if (stats.TaggedAllocations[i] >= MB) {
				unit[0] = 'M';
				amount = stats.TaggedAllocations[i] / (float)MB;
			}
			else if (stats.TaggedAllocations[i] >= KB) {
				unit[0] = 'K';
				amount = stats.TaggedAllocations[i] / (float)KB;
			}
			else {
				unit[0] = 'B';
				unit[1] = 0;
				amount = (float)stats.TaggedAllocations[i];
			}

			int32_t length = snprintf(buffer + offset, 8000, "  %s: %.2f%s\n", MemoryTagStrings[i], amount, unit);
			offset += length;
		}

		char* outString = _strdup(buffer);
		return outString;
	}

}	// Ocean
