#pragma once

#include "Ocean/Core/Types/Bool.hpp"

#include "Ocean/Core/Primitives/Memory.hpp"

namespace Ocean {

	namespace ADT {

		template<typename T>
		class List {
		public:
			List() : p_Allocator(nullptr), m_Size(0) { }
			virtual ~List() = default;

			virtual void Insert(const T& element, u32 position) = 0;
			virtual void Remove(u32 position) = 0;

			virtual i64 IndexOf(const T& element) const = 0;
			
			virtual T  Get(u32 position) const = 0;
			virtual T& Get(u32 position) = 0;

			T operator [] (u32 i) const {
				OASSERT_LENGTH(i, m_Size);

				return Get(i);
			}

			T& operator [] (u32 i) {
				OASSERT_LENGTH(i, m_Size);

				return Get(i);
			}

			virtual void Clear() = 0;

			u32 Size() const { return m_Size; }
			b8 IsEmpty() const { return m_Size == 0; }

		protected:
			Allocator* p_Allocator;

			u32 m_Size;

		};

	}	// ADT


	// TODO: Single Linked List Implementation
	template<typename T>
	class SingleLinkedList : public ADT::List<T> {
	private:
		struct Node {
			T data;

			Node* next;
		};

	public:
		SingleLinkedList() = default;
		virtual ~SingleLinkedList() = default;

		void Init();
		void Shutdown();

		virtual void Insert(const T& element, u32 position) override;
		virtual void Remove(u32 position) override;

		virtual i64 IndexOf(const T& element) const override;

		virtual T  Get(u32 position) const override;
		virtual T& Get(u32 position) override;

		virtual void Clear() override;

	private:
		Node* p_Head = nullptr;

	};



	template<typename T>
	class DoubleLinkedList : public ADT::List<T> {
	private:
		struct Node {
			T data;

			Node* next;
			Node* prev;
		};

	public:
		DoubleLinkedList() : ADT::List<T>() { }
		virtual ~DoubleLinkedList() = default;

		void Init();
		void Shutdown();

		virtual void Insert(const T& element, u32 position) override;
		virtual void Remove(u32 position) override;

		virtual i64 IndexOf(const T& element) const override;

		virtual T  Get(u32 position) const override;
		virtual T& Get(u32 position) override;

		virtual void Clear() override;

	private:
		Node* p_Head = nullptr;
		Node* p_Tail = nullptr;

	};



	template<typename T>
	inline void SingleLinkedList<T>::Init() {
		this->p_Allocator = MemoryService::Instance().SystemAllocator();
		this->m_Size = 0;
	}

	template<typename T>
	inline void SingleLinkedList<T>::Shutdown() {
		while (this->p_Head) {
			Node* temp = this->p_Head->next;

			ofree(this->p_Head, this->p_Allocator);

			this->p_Head = temp;
		}

		this->p_Head = nullptr;
	}

	template<typename T>
	inline void SingleLinkedList<T>::Insert(const T& element, u32 position) {
	}

	template<typename T>
	inline void SingleLinkedList<T>::Remove(u32 position) {
	}

	template<typename T>
	inline i64 SingleLinkedList<T>::IndexOf(const T& element) const {
		return u32();
	}

	template<typename T>
	inline T SingleLinkedList<T>::Get(u32 position) const {
		return T();
	}

	template<typename T>
	inline T& SingleLinkedList<T>::Get(u32 position) {
		// TODO: insert return statement here
	}

	template<typename T>
	inline void SingleLinkedList<T>::Clear() {
		while (this->p_Head) {
			Node* temp = this->p_Head->next;

			ofree(this->p_Head, this->p_Allocator);

			this->p_Head = temp;
		}

		this->p_Head = nullptr;
		this->m_Size = 0;
	}



	template<typename T>
	inline void DoubleLinkedList<T>::Init() {
		this->p_Allocator = MemoryService::Instance().SystemAllocator();
		this->m_Size = 0;
	}

	template<typename T>
	inline void DoubleLinkedList<T>::Shutdown() {
		while (this->p_Head) {
			Node* temp = this->p_Head->next;

			ofree(this->p_Head, this->p_Allocator);

			this->p_Head = temp;
		}

		this->p_Head = this->p_Tail = nullptr;
	}

	template<typename T>
	inline void DoubleLinkedList<T>::Insert(const T& element, u32 position) {
		OASSERT_LENGTH(position, this->m_Size);

		Node* temp = oallocat(Node, 1, this->p_Allocator);
		temp->data = element;

		if (this->p_Head == nullptr || position == 0) { // Front of list optimization.
			temp->next = this->p_Head;
			temp->prev = nullptr;

			if (this->p_Head)
				this->p_Head->prev = temp;
			else
				this->p_Tail = temp;

			this->p_Head = temp;
		}
		else if (position == this->m_Size) { // Back of list optimization.
			temp->next = nullptr;
			temp->prev = this->p_Tail;

			this->p_Tail->next = temp;
			this->p_Tail = temp;
		}
		else { // Anywhere else.
			Node* curr = nullptr;

			if (position <= (this->m_Size / 2)) {
				curr = this->p_Head;

				for (u32 i = 1; i < position; i++)
					curr = curr->next;

				temp->next = curr->next;
				temp->prev = curr;

				if (curr->next)
					curr->next->prev = temp;

				curr->next = temp;
			}
			else {
				curr = this->p_Tail;

				for (u32 i = this->m_Size - 1; i > position; i--)
					curr = curr->prev;

				temp->next = curr;
				temp->prev = curr->prev;

				if (curr->prev)
					curr->prev->next = temp;

				curr->prev = temp;
			}
		}

		this->m_Size++;
	}

	template<typename T>
	inline void DoubleLinkedList<T>::Remove(u32 position) {
		OASSERT_LENGTH(position, this->m_Size);

		Node* curr = nullptr;

		if (position == 0) {
			curr = this->p_Head;

			this->p_Head = this->p_Head->next;
		}
		else if (position == this->m_Size - 1) {
			curr = this->p_Tail;

			this->p_Tail = this->p_Tail->prev;
		}
		else {
			if (position <= (this->m_Size / 2)) {
				curr = this->p_Head;

				for (u32 i = 0; i < position; i++)
					curr = curr->next;
			}
			else {
				curr = this->p_Tail;

				for (u32 i = this->m_Size - 1; i > position; i--)
					curr = curr->prev;
			}

			if (curr->next)
				curr->next->prev = curr->prev;

			if (curr->prev)
				curr->prev->next = curr->next;
		}

		ofree(curr, this->p_Allocator);
		this->m_Size--;
	}

	template<typename T>
	inline i64 DoubleLinkedList<T>::IndexOf(const T& element) const {
		Node* curr = this->p_Head;

		for (u32 i = 0; i < this->m_Size; i++) {
			if (curr->data == element)
				return i;

			curr = curr->next;
		}

		return -1;
	}

	template<typename T>
	inline T DoubleLinkedList<T>::Get(u32 position) const {
		OASSERT_LENGTH(position, this->m_Size);

		Node* curr = nullptr;

		if (position <= (this->m_Size / 2)) {
			curr = this->p_Head;

			for (u32 i = 0; i < position; i++)
				curr = curr->next;
		}
		else {
			curr = this->p_Tail;

			for (u32 i = this->m_Size; i > position; i++)
				curr = curr->prev;
		}

		return curr->data;
	}

	template<typename T>
	inline T& DoubleLinkedList<T>::Get(u32 position) {
		OASSERT_LENGTH(position, this->m_Size);

		Node* curr = nullptr;

		if (position <= (this->m_Size / 2)) {
			curr = this->p_Head;

			for (u32 i = 0; i < position; i++)
				curr = curr->next;
		}
		else {
			curr = this->p_Tail;

			for (u32 i = this->m_Size; i > position; i++)
				curr = curr->prev;
		}

		return curr->data;
	}

	template<typename T>
	inline void DoubleLinkedList<T>::Clear() {
		while (this->p_Head) {
			Node* temp = this->p_Head->next;

			ofree(this->p_Head, this->p_Allocator);

			this->p_Head = temp;
		}

		this->p_Head = this->p_Tail = nullptr;
		this->m_Size = 0;
	}

}	// Ocean
