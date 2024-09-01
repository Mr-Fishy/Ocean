#pragma once

#include "Ocean/Core/Types/ValueTypes.hpp"

namespace Ocean {

	namespace ADT {

		template <typename T>
		class List {
		public:
			List() = default;
			virtual ~List() = default;

			virtual void Append(const T& element) = 0;
			virtual void Prepend(const T& element) = 0;
			virtual b8 InsertAfter(const T& element, const T& data) = 0;

			virtual void Remove(const T& element) = 0;

			virtual u32 IndexOf(const T& element) const = 0;
			virtual T* Get(u32 position) const = 0;

			virtual T* Front() const = 0;
			virtual T* Back() const = 0;

			virtual void Clear() = 0;

			virtual cstring ToString() const = 0;
			virtual b8 Equals(Object* rhs) const = 0;

		protected:
			u32 m_Size;

		};

	}	// ADT

}	// Ocean
