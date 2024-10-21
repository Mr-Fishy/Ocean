#pragma once

#include "Ocean/Core/Types/ValueTypes.hpp"

namespace Ocean {

	namespace ADT {

		template<typename T>
		class List {
		public:
			List() = default;
			virtual ~List() = default;

			virtual void Insert(const T& element, u32 position) = 0;
			virtual void Remove(u32 position) = 0;

			virtual u32 IndexOf(const T& element) const = 0;
			
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
			u32 m_Size;

		};

	}	// ADT



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

		virtual u32 IndexOf(const T& element) const override;

		virtual T  Get(u32 position) const override;
		virtual T& Get(u32 position) override;

		virtual void Clear() override;

	private:


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
		DoubleLinkedList() = default;
		virtual ~DoubleLinkedList() = default;

		void Init();
		void Shutdown();

		virtual void Insert(const T& element, u32 position) override;
		virtual void Remove(u32 position) override;

		virtual u32 IndexOf(const T& element) const override;

		virtual T  Get(u32 position) const override;
		virtual T& Get(u32 position) override;

		virtual void Clear() override;

	private:
	};

}	// Ocean
