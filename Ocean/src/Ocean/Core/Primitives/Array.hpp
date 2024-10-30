#pragma once

#include "Ocean/Core/Types/Bool.hpp"

#include "Ocean/Core/Primitives/Memory.hpp"
#include "Ocean/Core/Primitives/Assert.hpp"

// std
#include <cstring>
#include <sstream>

namespace Ocean {

	namespace ADT {

		// TODO: Implement Range-Based For Loop Functionality

		/**
		 * @brief The abstract array class.
		 * @tparam T - The type of data to store.
		 */
		template <typename T>
		class Array {
		public:
			Array() = default;
			~Array() { OASSERTM(this->p_Data == nullptr, CONSOLE_TEXT_RED("Array attempted deconstruction while data may still exist!")) }

			virtual void Set(u32 position, const T& element) = 0;
			
			virtual void Append(const T& element) = 0;
			virtual void Prepend(const T& element) = 0;

			/**
			 * @param element - The element to find.
			 * @return The index (position) of the element, returns -1 if the element is not found.
			 */
			i64 IndexOf(const T& element) const {
				for (u32 i = 0; i < this->m_Size; i++) {
					if (&this->p_Data[i] == &element)
						return i;
				}

				return -1;
			}
			/** 
			 * @param position - The position of the element to get.
			 * @return The pointer of the element at the given position. 
			 */
			T  Get(u32 position) const {
				OASSERTM(position < this->m_Size, "Attempt to get element out of range!")

				return this->p_Data[position];
			}
			/**
			 * @param position - The position of the element to get.
			 * @return The element at the given position.
			 */
			T& Get(u32 position) {
				OASSERTM(position < this->m_Size, "Attempt to get element out of range!")

				return this->p_Data[position];
			}

			T operator [] (u32 i) const {
				OASSERT_LENGTH(i, this->m_Capacity);

				return this->p_Data[i];
			}

			T& operator [] (u32 i) {
				OASSERT_LENGTH(i, this->m_Capacity);

				return this->p_Data[i];
			}

			/** 
			 * @return The pointer to the first element of the array.
			 */
			T  Front() const { return this->p_Data[0]; }
			/**
			 * @return The first element of the array.
			 */
			T& Front() { return this->p_Data[0]; }
			/**
			 * @return The pointer to the last element of the array.
			 */
			T  Back() const { return this->p_Data[this->m_Size - 1]; }
			/**
			 * @return The last element of the array.
			 */
			T& Back() { return this->p_Data[this->m_Size - 1]; }

			virtual void Remove(const T& element) = 0;
			virtual void Remove(u32 position) = 0;

			/**
			 * @return The pointer to the first element of the array.
			 */
			T* Data() const { return this->p_Data; }

			/**
			 * @brief Clear's the data within the array.
			 */
			void Clear() {
				if (this->p_Data)
					memset(this->p_Data, 0, this->m_Size);

				this->m_Size = 0;
			}

			/**
			 * @return The size of the array.
			 */
			u32 Size() const { return this->m_Size; }
			/**
			 * @brief Set's the size of the array if needed. Primary use case is for when a library inserts data from the Data() pointer.
			 * @brief Ex: Vulkan
			 * @param size - The size to set to the array.
			 */
			void SetSize(u32 size) { this->m_Size = size; }
			/**
			 * @return True if the array is empty, False otherwise.
			 */
			b8 IsEmpty() const { return this->m_Size == 0; }

		protected:
			Allocator* p_Allocator;

			T* p_Data;

			u32 m_Capacity, m_Size;
		};

	}	// ADT



	/**
	 * @brief A dynamic array like std::vector. Uses Ocean memory systems and contains space optimizations.
	 * @tparam T - The type of data to store.
	 */
	template <typename T>
	class DynamicArray : public ADT::Array<T> {
	public:
		DynamicArray() = default;
		DynamicArray(u32 capacity) { Init(capacity); }
		DynamicArray(const DynamicArray&);
		~DynamicArray() { Shutdown(); }
		
		/**
		 * @brief Initializes the array. The array must be shutdown before being re-initialized.
		 * @param initialCapacity - The initial capacity to allocate, default is 4.
		 */
		void Init(u32 initialCapacity = 4);
		/**
		 * @brief Shuts down the array. This must be done before the application is closed to release the data in memory.
		 */
		void Shutdown();

		/**
		 * @brief Set's an element at the given position to the given element. The position must be within the size of the array.
		 * @param position - The position to set.
		 * @param element - The element to set data[position] equal to.
		 */
		virtual void Set(u32 position, const T& element) override;

		/**
		 * @brief Append's the given element to the end of the array. Resizes the array if needed.
		 * @param element - The element to append.
		 */
		virtual void Append(const T& element) override;
		/**
		 * @brief Prepend's the given element to the beginning of the array. Resizes the array if needed.
		 * @param element - The element to prepend.
		 */
		virtual void Prepend(const T& element) override;

		/**
		 * @brief Removes the given element from the array.
		 * @param element - The element to remove.
		 */
		virtual void Remove(const T& element) override;
		/**
		 * @brief Removes the element at the given position.
		 * @param position - The position to remove at.
		 */
		virtual void Remove(u32 position) override;

		/**
		 * @brief Resizes the capacity of the array to the new given size.
		 * @param size - The new size to use.
		 */
		void Resize(u32 size);

		DynamicArray& operator = (const DynamicArray&);

	};

	// Implementation

	template<typename T>
	inline DynamicArray<T>::DynamicArray(const DynamicArray& array) {
		if (this != &array) {
			this->p_Allocator = array.p_Allocator;
			this->m_Capacity = array.m_Capacity;
			this->m_Size = array.m_Size;

			this->p_Data = oallocat(T, this->m_Capacity, this->p_Allocator);

			for (u32 i = 0; i < this->m_Size; i++)
				this->p_Data[i] = array.p_Data[i];
		}
	}

	template<typename T>
	inline void DynamicArray<T>::Init(u32 initialCapacity) {
		this->p_Allocator = MemoryService::Instance()->SystemAllocator();
		this->m_Capacity = initialCapacity;

		if (initialCapacity != 0) {
			this->p_Data = oallocat(T, this->m_Capacity, this->p_Allocator);
		}
		else
			this->p_Data = nullptr;

		this->m_Size = 0;
	}

	template<typename T>
	inline void DynamicArray<T>::Shutdown() {
		if (this->m_Capacity > 0)
			ofree(this->p_Data, this->p_Allocator);

		this->p_Data = nullptr;
		this->m_Size = this->m_Capacity = 0;
	}

	template<typename T>
	inline void DynamicArray<T>::Set(u32 position, const T& element) {
		OASSERTM(position < this->m_Size, "Attempt to set value out of array range!")
			
		this->p_Data[position] = element;
	}

	template<typename T>
	inline void DynamicArray<T>::Append(const T& element) {
		if (this->m_Size >= this->m_Capacity)
			Resize(this->m_Capacity + this->m_Capacity);

		this->p_Data[this->m_Size++] = element;
	}

	template<typename T>
	inline void DynamicArray<T>::Prepend(const T& element) {
		if (this->m_Size >= this->m_Capacity)
			Resize(this->m_Capacity + this->m_Capacity);

		for (u32 i = 1; i <= this->m_Size; i++)
			this->p_Data[this->m_Size - i + 1] = this->p_Data[this->m_Size - i];

		this->p_Data[0] = element;
		this->m_Size++;
	}

	template<typename T>
	inline void DynamicArray<T>::Remove(const T& element) {
		for (u32 i = 0; i < this->m_Size; i++) {
			if (&this->p_Data[i] == &element) {
				this->p_Data[i] = this->p_Data[--this->m_Size];

				return;
			}
		}
	}

	template<typename T>
	inline void DynamicArray<T>::Remove(u32 position) {
		if (position < this->m_Size)
			memset(&this->p_Data[position], 0, sizeof(T));

		this->m_Size--;
	}

	template<typename T>
	inline DynamicArray<T>& DynamicArray<T>::operator = (const DynamicArray& array) {
		if (this != &array) {
			this->p_Allocator = array.p_Allocator;
			this->m_Capacity = array.m_Capacity;
			this->m_Size = array.m_Size;

			this->p_Data = oallocat(T, this->m_Capacity, this->p_Allocator);

			mem_copy(this->p_Data, array.p_Data, array.m_Size * sizeof(T));
		}

		return *this;
	}

	template<typename T>
	inline void DynamicArray<T>::Resize(u32 newCapacity) {
		T* newData = oallocat(T, this->m_Capacity, this->p_Allocator);

		if (this->m_Capacity) {
			mem_copy(newData, this->p_Data, this->m_Capacity * sizeof(T));

			ofree(this->p_Data, this->p_Allocator);
		}

		this->p_Data = newData;
		this->m_Capacity = newCapacity;
	}



	template <typename T>
	class FixedArray : public ADT::Array<T> {
	public:
		FixedArray() = default;
		FixedArray(u32 capacity) { Init(capacity); this->SetSize(capacity); }
		FixedArray(const FixedArray&);
		~FixedArray() { Shutdown(); }

		/**
		 * @brief Initializes the array. The array must be shutdown before being re-initialized.
		 * @param capacity - The capacity to allocate, default is 0.
		 */
		void Init(u32 capacity = 0);
		/**
		 * @brief Shuts down the array. This must be done before the application is closed to release the data in memory.
		 */
		void Shutdown();

		/**
		 * @brief Set's an element at the given position to the given element. The position must be within the size of the array.
		 * @param position - The position to set.
		 * @param element - The element to set data[position] equal to.
		 */
		virtual void Set(u32 position, const T& element) override;

		/**
		 * @brief Append's the given element to the end of the array. There must be space available.
		 * @param element - The element to append.
		 */
		virtual void Append(const T& element) override;
		/**
		 * @brief Prepend's the given element to the beginning of the array. There must be space available.
		 * @param element - The element to prepend.
		 */
		virtual void Prepend(const T& element) override;

		/**
		 * @brief Removes the given element from the array.
		 * @param element - The element to remove.
		 */
		virtual void Remove(const T& element) override;
		/**
		 * @brief Removes the element at the given position.
		 * @param position - The position to remove at.
		 */
		virtual void Remove(u32 position) override;

		FixedArray& operator = (const FixedArray&);

	};

	// Implementation

	template<typename T>
	inline FixedArray<T>::FixedArray(const FixedArray& array) {
		if (this != &array) {
			this->p_Allocator = array.p_Allocator;
			this->m_Capacity = array.m_Capacity;
			this->m_Size = array.m_Size;

			this->p_Data = oallocat(T, this->m_Capacity, this->p_Allocator);

			mem_copy(this->p_Data, array.p_Data, array.m_Size * sizeof(T));
		}
	}

	template<typename T>
	inline void FixedArray<T>::Init(u32 capacity) {
		this->p_Allocator = MemoryService::Instance()->SystemAllocator();
		this->m_Capacity = capacity;

		if (capacity != 0) {
			this->p_Data = oallocat(T, this->m_Capacity, this->p_Allocator);
		}
		else
			this->p_Data = nullptr;

		this->m_Size = 0;
	}

	template<typename T>
	inline void FixedArray<T>::Shutdown() {
		ofree(this->p_Data, this->p_Allocator);

		this->p_Data = nullptr;
		this->m_Size = this->m_Capacity = 0;
	}

	template<typename T>
	inline void FixedArray<T>::Append(const T& element) {
		OASSERTM(this->m_Size < this->m_Capacity, "Fixed Array :| Attempted to append element to a full array!")

		this->p_Data[this->m_Size++] = element;
	}

	template<typename T>
	inline void FixedArray<T>::Prepend(const T& element) {
		OASSERTM(this->m_Size < this->m_Capacity, "Fixed Array :| Attempted to append element to a full array!")

		for (u32 i = 1; i <= this->m_Size; i++)
			this->p_Data[this->m_Size - i + 1] = this->p_Data[this->m_Size - i];

		this->p_Data[0] = element;
		this->m_Size++;
	}

	template<typename T>
	inline void FixedArray<T>::Set(u32 position, const T& element) {
		OASSERTM(position < this->m_Capacity, "Fixed Array :| Attempted to set element value outside the range!")

		this->p_Data[position] = element;
	}

	template<typename T>
	inline void FixedArray<T>::Remove(const T& element) {
		for (u32 i = 0; i < this->m_Size; i++) {
			if (&this->p_Data[i] == &element) {
				this->p_Data[i] = this->p_Data[--this->m_Size];

				return;
			}
		}
	}

	template<typename T>
	inline void FixedArray<T>::Remove(u32 position) {
		if (position < this->m_Size)
			memset(&this->p_Data[position], 0, sizeof(T));

		this->m_Size--;
	}

	template<typename T>
	inline FixedArray<T>& FixedArray<T>::operator = (const FixedArray& array) {
		if (this != &array) {
			this->p_Allocator = array.p_Allocator;
			this->m_Capacity = array.m_Capacity;
			this->m_Size = array.m_Size;

			this->p_Data = oallocat(T, this->m_Capacity, this->p_Allocator);

			mem_copy(this->p_Data, array.p_Data, array.m_Size * sizeof(T));
		}

		return *this;
	}

}	// Ocean
