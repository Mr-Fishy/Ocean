#pragma once

#include "Ocean/Core/Primitives/Memory.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"

#include "Ocean/Core/Types/Bool.hpp"

// std
#include <sstream>

namespace Ocean {

	namespace ADT {

		template <typename T>
		class Array {
		public:
			Array() = default;
			~Array() = default;

			virtual void InsertAt(const T& element, u32 position) = 0;
			virtual b8 InsertAfter(const T& element, const T& data) = 0;
			
			virtual void Append(const T& element) = 0;
			virtual void Prepend(const T& element) = 0;

			virtual i64 IndexOf(const T& element) const = 0;
			virtual T* Get(u32 position) const = 0;

			virtual T* Front() const = 0;
			virtual T* Back() const = 0;

			virtual void Remove(const T& element) = 0;
			virtual void Remove(u32 position) = 0;

			T* Data() const { return p_Data; }

			virtual void Clear() = 0;

			u32 Size() const { return m_Size; }
			void SetSize(u32 size) { m_Size = size; } // This is a temporary fix to handle Data() useage where it doesn't know it has new data yet.

			virtual cstring ToString() const { return "Array ADT"; }

		protected:
			Allocator* p_Allocator = nullptr;

			T* p_Data = nullptr;

			u32 m_Capacity, m_Size;
		};

	}	// ADT



	template <typename T>
	class DynamicArray : public ADT::Array<T> {
	public:
		DynamicArray() = default;
		~DynamicArray() = default;

		void Init(Allocator* allocator, u32 initialCapacity = 4);
		void Shutdown();

		virtual void InsertAt(const T& element, u32 position) override;
		virtual b8 InsertAfter(const T& element, const T& data) override;

		virtual void Append(const T& element) override;
		virtual void Prepend(const T& element) override;

		virtual i64 IndexOf(const T& element) const override;
		virtual T* Get(u32 position) const override;

		virtual T* Front() const override;
		virtual T* Back() const override;

		virtual void Remove(const T& element) override;
		virtual void Remove(u32 position) override;

		virtual void Clear() override;

		void Grow(u32 amount);

		virtual cstring ToString() const override;

		friend std::ostream& operator << (std::ostream& os, const DynamicArray& obj);

	private:
		DynamicArray(const DynamicArray&);
		const DynamicArray& operator = (const DynamicArray&) { }

		void Resize(u32 newSize);

	};

	template <typename T>
	class FixedArray : public ADT::Array<T> {
	public:
		void Init(Allocator* allocator, u32 capacity);
		void Shutdown();

		virtual void InsertAt(const T& element, u32 position) override;
		virtual b8 InsertAfter(const T& element, const T& data) override;

		virtual void Append(const T& element) override;
		virtual void Prepend(const T& element) override;

		virtual i64 IndexOf(const T& element) const override;
		virtual T* Get(u32 position) const override;

		virtual T* Front() const override;
		virtual T* Back() const override;

		virtual void Remove(const T& element) override;
		virtual void Remove(u32 position) override;

		virtual void Clear() override;

		virtual cstring ToString() const override;

		friend std::ostream& operator << (std::ostream& os, const FixedArray& obj);

	private:
		FixedArray(const FixedArray&);
		const FixedArray& operator = (const FixedArray&) { }

	};

	// Implementation

	template<typename T>
	inline void DynamicArray<T>::Init(Allocator* allocator, u32 initialCapacity) {
		p_Allocator = allocator;
		m_Capacity = initialCapacity;

		p_Data = (T*)p_Allocator->Allocate(m_Capacity * sizeof(T), alignof(T));

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
	inline b8 DynamicArray<T>::InsertAfter(const T& element, const T& data) {
		if (m_Size >= m_Capacity)
			Resize(m_Capacity + m_Capacity);

		for (u32 i = 0; i < m_Size; i++) {
			if (p_Data[i] == element) {
				for (u32 j = i + 1; j <= m_Size; j++)
					p_Data[m_Size - j + 1] = p_Data[m_Size - j];

				p_Data[i] = element;
				m_Size++;

				return true;
			}
		}

		return false;
	}

	template<typename T>
	inline void DynamicArray<T>::Remove(const T& element) {
		for (u32 i = 0; i < m_Size; i++) {
			if (p_Data[i] == element) {
				p_Data[i] = p_Data[--m_Size];

				return;
			}
		}
	}

	template<typename T>
	inline void DynamicArray<T>::Remove(u32 position) {
		if (position < m_Size)
			p_Data[position] = T();

		m_Size--;
	}

	template<typename T>
	inline i64 DynamicArray<T>::IndexOf(const T& element) const {
		for (u32 i = 0; i < m_Size; i++) {
			if (p_Data[i] == element)
				return i;
		}
		
		return -1;
	}

	template<typename T>
	inline T* DynamicArray<T>::Get(u32 position) const {
		if (position >= m_Size)
			return nullptr;

		return &p_Data[position];
	}

	template<typename T>
	inline T* DynamicArray<T>::Front() const {
		return &p_Data[0];
	}

	template<typename T>
	inline T* DynamicArray<T>::Back() const {
		return &p_Data[m_Size - 1];
	}

	template<typename T>
	inline void DynamicArray<T>::Clear() {
		memset(p_Data, 0, m_Size);

		m_Size = 0;
	}

	template<typename T>
	inline void DynamicArray<T>::Grow(u32 amount) {
		if (amount < 4)
			amount = 4;

		Resize(m_Capacity + amount);
	}

	template<typename T>
	inline cstring DynamicArray<T>::ToString() const {
		std::stringstream ss;

		ss << "DynamicArray: {";

		for (u32 i = 0; i < m_Size - 1; i++)
			ss << "Index: " << i << ", Data: " << p_Data[i] << ", ";

		ss << "Index: " << m_Size - 1 << ", Data: " << p_Data[m_Size - 1] << "}";

		return ss.str().c_str();
	}

	template <typename T>
	std::ostream& operator << (std::ostream& os, const DynamicArray<T>& obj) {
		return os << obj.ToString();
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



	template<typename T>
	inline void FixedArray<T>::Init(Allocator* allocator, u32 capacity) {
		p_Allocator = allocator;
		m_Capacity = capacity;

		p_Data = (T*)p_Allocator->Allocate(m_Capacity * sizeof(T), alignof(T));

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
	inline b8 FixedArray<T>::InsertAfter(const T& element, const T& data) {
		OASSERTM(m_Size < m_Capacity, "Fixed Array :| Attempted to append element to a full array!");

		for (u32 i = 0; i < m_Size; i++) {
			if (p_Data[i] == element) {
				for (u32 j = i + 1; j <= m_Size; j++)
					p_Data[m_Size - j + 1] = p_Data[m_Size - j];

				p_Data[i] = element;
				m_Size++;

				return true;
			}
		}

		return false;
	}

	template<typename T>
	inline void FixedArray<T>::Remove(const T& element) {
		for (u32 i = 0; i < m_Size; i++) {
			if (p_Data[i] == element) {
				p_Data[i] = p_Data[--m_Size];

				return;
			}
		}
	}

	template<typename T>
	inline void FixedArray<T>::Remove(u32 position) {
		if (position < m_Size)
			p_Data[position] = T();

		m_Size--;
	}

	template<typename T>
	inline i64 FixedArray<T>::IndexOf(const T& element) const {
		for (u32 i = 0; i < m_Size; i++) {
			if (p_Data[i] == element)
				return i;
		}

		return -1;
	}

	template<typename T>
	inline T* FixedArray<T>::Get(u32 position) const {
		if (position >= m_Size)
			return nullptr;

		return &p_Data[position];
	}

	template<typename T>
	inline T* FixedArray<T>::Front() const {
		return &p_Data[0];
	}

	template<typename T>
	inline T* FixedArray<T>::Back() const {
		return &p_Data[m_Size - 1];
	}

	template<typename T>
	inline void FixedArray<T>::Clear() {
		memset(p_Data, 0, m_Size);

		m_Size = 0;
	}

	template<typename T>
	inline cstring FixedArray<T>::ToString() const {
		std::stringstream ss;

		ss << "FixedArray: {";

		for (u32 i = 0; i < m_Size - 1; i++)
			ss << "Index: " << i << ", Data: " << p_Data[i] << ", ";

		ss << "Index: " << m_Size - 1 << ", Data: " << p_Data[m_Size - 1] << "}";

		return ss.str().c_str();
	}

	template <typename T>
	std::ostream& operator << (std::ostream& os, const FixedArray<T>& obj) {
		return os << obj.ToString();
	}

}	// Ocean
