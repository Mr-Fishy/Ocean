#pragma once

namespace Ocean {

	template <class T>
	class SingleLinkedList {
	public:
		struct Node {
			T Data;

			Node* Next;
		};

		/* --- */

		SingleLinkedList() = default;

		inline void Insert(Node* prevNode, Node* newNode) {
			if (prevNode == nullptr) {
				if (m_Head != nullptr)
					newNode->Next = m_Head;
				else
					newNode->Next = nullptr;

				m_Head = newNode;
			}
			else {
				if (prevNode->Next == nullptr) {
					prevNode->Next = newNode;

					newNode->Next = nullptr;
				}
				else {
					newNode->Next = prevNode->Next;

					prevNode->Next = newNode;
				}
			}
		}

		inline void Remove(Node* prevNode, Node* deleteNode) {
			if (prevNode == nullptr) {
				if (deleteNode->Next == nullptr)
					m_Head = nullptr;
				else
					m_Head = deleteNode->Next;
			}
			else {
				prevNode->Next = deleteNode->Next;
			}
		}

		Node* m_Head;
	};

}	// Ocean
