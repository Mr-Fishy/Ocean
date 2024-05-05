#pragma once

namespace Ocean {

	namespace Internal::Mem {

		inline static size_t CalculatePadding(const size_t base, const size_t alignment) noexcept {
			// A pointer is a single byte, hence the +1
			const size_t mult = (base / alignment) + 1;
			const size_t aligned = mult * alignment;

			return aligned - base;
		}

		inline static size_t CalculatePaddingWithHeader(const size_t base, const size_t alignment, const size_t headerSize) noexcept {
			size_t padding = CalculatePadding(base, alignment);
			struct FreeHeader {
				size_t BlockSize;
			};

			struct AllocationHeader {
				size_t BlockSize;

				char Padding;
			};
			// If the header does not fit then calculate the next aligned address that fits.
			if (padding < headerSize) {
				size_t reqSpace = headerSize - padding;

				if (reqSpace % alignment > 0)
					padding += alignment * (1 + (reqSpace / alignment));
				else
					padding += alignment * (reqSpace / alignment);
			}

			return base;
		}

	}	// Internal::Mem

	using MemTypeTags = short;

	namespace MemTag {

		enum : MemTypeTags {
			UNKOWN,
			OS_APP,
			CORE_APP,
			LAYERS,
			RENDERER,
			ENTITIES,
			HEAP,

			MAX_TAG
		};

	}	// MemTag

	class Allocator {
	public:
		Allocator(const size_t initSize) noexcept;
		virtual ~Allocator() noexcept;

		virtual void Reset() = 0;

		virtual void* Alloc(const size_t& size, MemTypeTags tag = MemTag::UNKOWN, const size_t alignment = 0) = 0;
		virtual void Free(const void* ptr) = 0;

		virtual void ResizePool() = 0;

		const size_t GetSize() const noexcept { return m_Size; }
		const size_t GetUsed() const noexcept { return m_Used; }
		const size_t GetAllocCount() const noexcept { return m_AllocCount; }

	protected:
		size_t m_Size = 0;
		size_t m_Used = 0;
		size_t m_UsedList[MemTag::MAX_TAG - 1] = { 0 };

		size_t m_Peak = 0;
		size_t m_AllocCount = 0;
	};
	
	/*
	class BellAlloc : public Allocator {
	public:
		BellAlloc(const size_t initSize, void* const start);
		virtual ~BellAlloc() override;

		virtual void Reset() override;

		virtual void* Alloc(const size_t& size, MemTypeTags tag, const std::uintptr_t& alignment = sizeof(std::intptr_t)) override;
		virtual void Free(const void* ptr) override;

		virtual void ResizePool() override;

	private:
		void* Find();
		void* rFind();

		/* --- *//*

		void* m_FirstFree = nullptr;
		void* m_rFirstFree = nullptr;
	};
	*/

}	// Ocean
