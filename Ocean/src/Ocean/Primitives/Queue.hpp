#pragma once

#include "Ocean/Core/Primitives/List.hpp"

namespace Ocean {

	template<typename T>
	class Queue {
	public:
		void Init() { m_List.Init(); }
		void Shutdown() { m_List.Shutdown(); }

		void Insert(const T& element) { m_List.Insert(element, m_List.Size()); }
		void Pop() { m_List.Remove(0); }

		T  Peek() const { m_List.Get(0); }
		T& Peek() { m_List.Get(0); }

		void Clear() { m_List.Clear(); }

		u32 Size() const { return m_List.Size(); }

	private:
		DoubleLinkedList<T> m_List;

	};

	template<typename T>
	class PriorityQueue {
	public:
		void Init() { m_List.Init(); }
		void Shutdown() { m_List.Shutdown(); }

		void Insert(const T& element, u8 priority = 1) {
			m_List.Insert(element, m_List.Size() / priority);
		}
		void Pop() { m_List.Remove(0); }

		T  Peek() const { m_List.Get(0); }
		T& Peek() { m_List.Get(0); }

		void Clear() { m_List.Clear(); }

		u32 Size() const { return m_List.Size(); }

	private:
		DoubleLinkedList<T> m_List;

	};

}	// Ocean
