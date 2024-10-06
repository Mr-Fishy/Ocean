#pragma once

#include "Ocean/Core/Primitives/Memory.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"

#include "Ocean/Core/Types/Bool.hpp"

// std
#include <sstream>

namespace Ocean {

	namespace ADT {

		// TODO: Implement Range-Based For Loop Functionality

		template <typename T>
		class Array {
		public:
			Array() = default;
			~Array() { OASSERTM(!p_Data, "Array attempted deconstruction while data may still exist!"); }

			virtual void InsertAt(const T& element, u32 position) = 0;
			virtual void Set(u32 position, const T& element) = 0;
			
			virtual void Append(const T& element) = 0;
			virtual void Prepend(const T& element) = 0;

			i64 IndexOf(const T& element) const {
				for (u32 i = 0; i < m_Size; i++) {
					if (&p_Data[i] == &element)
						return i;
				}

				return -1;
			}
			T  Get(u32 position) const {
				OASSERTM(position < m_Size, "Attempt to get element out of range!");

				return p_Data[position];
			}
			T& Get(u32 position) {
				OASSERTM(position < m_Size, "Attempt to get element out of range!");

				return p_Data[position];
			}

			T  Front() const { return p_Data[0]; }
			T& Front() { return p_Data[0]; }
			T  Back() const { return p_Data[m_Size - 1]; }
			T& Back() { return p_Data[m_Size - 1]; }

			virtual void Remove(const T& element) = 0;
			virtual void Remove(u32 position) = 0;

			T* Data() const { return p_Data; }

			void Clear() {
				if (p_Data)
					memset(p_Data, 0, m_Size);

				m_Size = 0;
			}

			u32 Size() const { return m_Size; }
			void SetSize(u32 size) { m_Size = size; } // This is a temporary fix to handle Data() useage where it doesn't know it has new data yet.

		protected:
			Allocator* p_Allocator;

			T* p_Data;

			u32 m_Capacity, m_Size;
		};

	}	// ADT



	template <typename T>
	class DynamicArray : public ADT::Array<T> {
	public:
		DynamicArray() = default;
		DynamicArray(u32 capacity) { Init(capacity); }
		DynamicArray(const DynamicArray&);
		~DynamicArray() = default;
		
		void Init(u32 initialCapacity = 4);
		void Shutdown();

		virtual void InsertAt(const T& element, u32 position) override;
		virtual void Set(u32 position, const T& element) override;

		virtual void Append(const T& element) override;
		virtual void Prepend(const T& element) override;

		virtual void Remove(const T& element) override;
		virtual void Remove(u32 position) override;

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

			this->p_Data = (T*)this->p_Allocator->Allocate(m_Capacity * sizeof(T), alignof(T));

			for (u32 i = 0; i < this->m_Size; i++)
				this->p_Data[i] = array.p_Data[i];
		}
	}

	template<typename T>
	inline void DynamicArray<T>::Init(u32 initialCapacity) {
		p_Allocator = MemoryService::Instance()->SystemAllocator();
		m_Capacity = initialCapacity;

		if (initialCapacity != 0)
			p_Data = (T*)p_Allocator->Allocate(m_Capacity * sizeof(T), alignof(T));
		else
			p_Data = nullptr;

		m_Size = 0;
	}

	template<typename T>
	inline void DynamicArray<T>::Shutdown() {
		if (m_Capacity > 0)
			p_Allocator->Deallocate(p_Data);

		p_Data = nullptr;
		m_Size = m_Capacity = 0;
	}

	template<typename T>
	inline void DynamicArray<T>::InsertAt(const T& element, u32 position) {
		if (position >= m_Capacity)
			Resize(position);	// Resize to position (This can be dangerous but oh well)

		p_Data[position] = element;
	}

	template<typename T>
	inline void DynamicArray<T>::Set(u32 position, const T& element) {
		if (position >= m_Capacity)
			Resize(position);

		p_Data[position] = element;
	}

	template<typename T>
	inline void DynamicArray<T>::Append(const T& element) {
		if (m_Size >= m_Capacity)
			Resize(m_Capacity + m_Capacity);

		p_Data[m_Size++] = element;
	}

	template<typename T>
	inline void DynamicArray<T>::Prepend(const T& element) {
		if (m_Size >= m_Capacity)
			Resize(m_Capacity + m_Capacity);

		for (u32 i = 1; i <= m_Size; i++)
			p_Data[m_Size - i + 1] = p_Data[m_Size - i];

		p_Data[0] = element;
		m_Size++;
	}

	template<typename T>
	inline void DynamicArray<T>::Remove(const T& element) {
		for (u32 i = 0; i < m_Size; i++) {
			if (&p_Data[i] == &element) {
				p_Data[i] = p_Data[--m_Size];

				return;
			}
		}
	}

	template<typename T>
	inline void DynamicArray<T>::Remove(u32 position) {
		if (position < m_Size)
			memset(&p_Data[position], 0, sizeof(T));

		m_Size--;
	}

	template<typename T>
	inline DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray& array) {
		if (this != &array) {
			this->p_Allocator = array.p_Allocator;
			this->m_Capacity = array.m_Capacity;
			this->m_Size = array.m_Size;

			this->p_Data = (T*)this->p_Allocator->Allocate(m_Capacity * sizeof(T), alignof(T));

			for (u32 i = 0; i < this->m_Size; i++)
				this->p_Data[i] = array.p_Data[i];
		}

		return *this;
	}

	template<typename T>
	inline void DynamicArray<T>::Resize(u32 newCapacity) {
		T* newData = (T*)p_Allocator->Allocate(newCapacity * sizeof(T), alignof(T));

		if (m_Capacity) {
			mem_copy(newData, p_Data, m_Capacity * sizeof(T));

			p_Allocator->Deallocate(p_Data);
		}

		p_Data = newData;
		m_Capacity = newCapacity;
	}



	template <typename T>
	class FixedArray : public ADT::Array<T> {
	public:
		FixedArray(u32 capacity) { Init(capacity); }
		FixedArray(const FixedArray&);
		~FixedArray() = default;

		void Init(u32 capacity = 0);
		void Shutdown();

		virtual void InsertAt(const T& element, u32 position) override;
		virtual void Set(u32 position, const T& element) override;

		virtual void Append(const T& element) override;
		virtual void Prepend(const T& element) override;

		virtual void Remove(const T& element) override;
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

			this->p_Data = (T*)this->p_Allocator->Allocate(m_Capacity * sizeof(T), alignof(T));

			for (u32 i = 0; i < this->m_Size; i++)
				this->p_Data[i] = array.p_Data[i];
		}
	}

	template<typename T>
	inline void FixedArray<T>::Init(u32 capacity) {
		p_Allocator = MemoryService::Instance()->SystemAllocator();
		m_Capacity = capacity;

		if (capacity != 0)
			p_Data = (T*)p_Allocator->Allocate(m_Capacity * sizeof(T), alignof(T));
		else
			p_Data = nullptr;

		m_Size = 0;
	}

	template<typename T>
	inline void FixedArray<T>::Shutdown() {
		p_Allocator->Deallocate(p_Data);

		p_Data = nullptr;
		m_Size = m_Capacity = 0;
	}

	template<typename T>
	inline void FixedArray<T>::Append(const T& element) {
		OASSERTM(m_Size < m_Capacity, "Fixed Array :| Attempted to append element to a full array!");

		p_Data[m_Size++] = element;
	}

	template<typename T>
	inline void FixedArray<T>::Prepend(const T& element) {
		OASSERTM(m_Size < m_Capacity, "Fixed Array :| Attempted to append element to a full array!");

		for (u32 i = 1; i <= m_Size; i++)
			p_Data[m_Size - i + 1] = p_Data[m_Size - i];

		p_Data[0] = element;
		m_Size++;
	}

	template<typename T>
	inline void FixedArray<T>::InsertAt(const T& element, u32 position) {
		if (position < m_Capacity)
			p_Data[position] = element;
	}

	template<typename T>
	inline void FixedArray<T>::Set(u32 position, const T& element) {
		OASSERTM(position < m_Capacity, "Fixed Array :| Attempted to set element value outside the range!");

		p_Data[position] = element;
	}

	template<typename T>
	inline void FixedArray<T>::Remove(const T& element) {
		for (u32 i = 0; i < m_Size; i++) {
			if (&p_Data[i] == &element) {
				p_Data[i] = p_Data[--m_Size];

				return;
			}
		}
	}

	template<typename T>
	inline void FixedArray<T>::Remove(u32 position) {
		if (position < m_Size)
			memset(&p_Data[position], 0, sizeof(T));

		m_Size--;
	}

	template<typename T>
	inline FixedArray<T>& FixedArray<T>::operator = (const FixedArray& array) {
		if (this != &array) {
			this->p_Allocator = array.p_Allocator;
			this->m_Capacity = array.m_Capacity;
			this->m_Size = array.m_Size;

			this->p_Data = (T*)this->p_Allocator->Allocate(m_Capacity * sizeof(T), alignof(T));

			for (u32 i = 0; i < this->m_Size; i++)
				this->p_Data[i] = array.p_Data[i];
		}

		return *this;
	}

}	// Ocean
