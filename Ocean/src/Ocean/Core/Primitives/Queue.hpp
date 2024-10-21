#pragma once

#include "Ocean/Core/Primitives/List.hpp"

namespace Ocean {
	
	template<typename T>
	class Queue {
	public:
		void Init();
		void Shutdown();

		void Insert(T& element);
		void Pop();

		T  Peek() const;
		T& Peek();

		void Clear();

		u32 Size() const { return m_List.Size(); }

	private:
		DoubleLinkedList<T> m_List;

	};

}	// Ocean
