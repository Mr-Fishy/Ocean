#pragma once

namespace Ocean {

	template <class T>
	class StackLinkedList {
	public:
		struct Node {
			T Data;

			Node* Next;
		};

		/* --- */

		StackLinkedList() = default;

		inline void Push(Node* newNode) {
			newNode->Next = m_Head;
			
			m_Head = newNode;
		}

		inline Node* Pop() {
			Node* top = m_Head;
			m_Head = m_Head->Next;

			return top;
		}

		Node* m_Head;
	};

}	// Ocean
