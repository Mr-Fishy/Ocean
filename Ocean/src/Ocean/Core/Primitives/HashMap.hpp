#pragma once

#include "Ocean/Core/Types/Bool.hpp"
#include "Ocean/Core/Primitives/Memory.hpp"
#include "Ocean/Core/Primitives/Assert.hpp"

namespace Ocean {

	namespace ADT {

		template <typename K, typename T>
		class Map {
		public:
			Map() = default;
			virtual ~Map() = default;

			virtual void Insert(K key, T value) = 0;
			virtual void Remove(const K& key) = 0;

			virtual T* Get(const K& key) = 0;
			virtual void Set(const K& key, const T& value) = 0;

			virtual void Clear() = 0;
			virtual void Reserve(u32 amount) = 0;

			/**
			 * @return The size of the HasMap.
			 */
			u32 Size() const { return m_Size; }
			/**
			 * @return True if the HasMap is empty, False otherwise.
			 */
			b8 IsEmpty() const { return m_Size == 0; }

		protected:
			Allocator* p_Allocator = nullptr;

			u32 m_Size = 0;
			u32 m_Capacity = 0;

		};

	}	// ADT

	// TODO: Iterators
	// TODO: Backfeed Deleting

	template <typename K, typename T>
	class HashMap : public ADT::Map<K, T> {
	private:
		struct KeyValue {
			K Key;
			T Value;

			u32 Hash;

		};	// KeyValue

	public:
		HashMap() = default;
		virtual ~HashMap() = default;

		/**
		 * @brief Initializes the HashMap. Shutdown() must be called before re-initializing.
		 * @param allocator - The Ocean memory allocator to use.
		 * @param initialCapacity - The initial capacity of the map.
		 */
		void Init(Allocator* allocator, u32 initialCapacity);
		/**
		 * @brief Shuts down the HashMap. This must be called before the application is closed if initialized.
		 */
		void Shutdown();

		/**
		 * @brief Insert's an element with the given Key.
		 * @param key - The key to use in the map.
		 * @param value - The value to insert.
		 */
		virtual void Insert(K key, T value) override;
		/**
		 * @brief Remove's an element from the HashMap.
		 * @param key - The key of the element to remove.
		 */
		virtual void Remove(const K& key) override;

		/**
		 * @param key - The key of the element to get.
		 * @return The pointer to the element if it is found, nullptr otherwise.
		 */
		virtual T* Get(const K& key) override;
		/**
		 * @brief Set's the value of the element with the given key.
		 * @param key - The key of the element to set.
		 * @param value - The value to assign to the element.
		 */
		virtual void Set(const K& key, const T& value) override;
		/**
		 * @brief Finds if an element is in the HashMap with the given Key. 
		 * @param key - The key to search for.
		 * @return True if the element exists, False otherwise.
		 */
		b8 Find(const K& key);
		// Find and return iterator

		/**
		 * @brief Clear's the HashMap.
		 */
		virtual void Clear() override;
		/**
		 * @brief Reserve's the given amount in the HashMap to be available.
		 * @param amount - The ammount of elements to reserve.
		 */
		virtual void Reserve(u32 amount) override;

	private:
		u32 HashKey(const K& key);
		u32 DesiredPos(u32 hash);

		u32 ProbeDist(const u32& hash, const u32& pos);
		b8 IsDeleted(u32 hash);

		i32 LookupIndex(const u32& hash, const K& key);

		void InternalInsert(u32 hash, K& key, T& value);

		void Resize(u32 newCapacity);

		/* --- */

		u32 m_Threshold = 0;
		u32 m_Mask = 0;

		KeyValue* p_Slots = nullptr;

	};

	// Implementation

	template<typename K, typename T>
	inline void HashMap<K, T>::Init(Allocator* allocator, u32 initialCapacity) {
		p_Allocator = allocator;
		m_Capacity = initialCapacity < 4 ? 4 : initialCapacity;
		m_Size = 0;
		m_Threshold = (m_Capacity * 9) / 10;
		m_Mask = m_Capacity - 1;

		p_Slots = (KeyValue*)p_Allocator->Allocate(sizeof(KeyValue) * m_Capacity, alignof(KeyValue));
	}

	template<typename K, typename T>
	inline void HashMap<K, T>::Shutdown() {
		p_Allocator->Deallocate(p_Slots);
	}

	template<typename K, typename T>
	inline void HashMap<K, T>::Insert(K key, T value) {
		if (++m_Size >= m_Threshold)
			Resize(m_Capacity + m_Capacity);

		InternalInsert(HashKey(key), key, value);
	}

	template<typename K, typename T>
	inline void HashMap<K, T>::Remove(const K& key) {
		const i32 i = LookupIndex(HashKey(key), key);

		if (i == -1)
			return;

		p_Slots[i].Hash |= 0x80000000; // Mark as deleted
		m_Size--;
	}

	template<typename K, typename T>
	inline T* HashMap<K, T>::Get(const K& key) {
		const i32 i = LookupIndex(HashKey(key), key);

		return i != -1 ? &p_Slots[i].Value : nullptr;
	}

	template<typename K, typename T>
	inline void HashMap<K, T>::Set(const K& key, const T& value) {
		const i32 i = LookupIndex(HashKey(key), key);

		if (i == -1)
			return;

		p_Slots[i].Value = value;
	}

	template<typename K, typename T>
	inline b8 HashMap<K, T>::Find(const K& key) {
		const i32 i = LookupIndex(HashKey(key), key);

		return i != -1;
	}

	template<typename K, typename T>
	inline void HashMap<K, T>::Clear() {
		for (u32 i = 0; i < m_Capacity; i++)
			p_Slots[i].Hash |= 0x80000000; // Mark as deleted

		m_Size = 0;
	}

	template<typename K, typename T>
	inline void HashMap<K, T>::Reserve(u32 amount) {
		Resize(amount < 4 ? 4 : amount);
	}

	template<typename K, typename T>
	inline u32 HashMap<K, T>::HashKey(const K& key) {
		const std::hash<K> hasher{ };
		u32 h = static_cast<u32>(hasher(key));

		// MSB is used to indicate a deleted elem, so
		// clear it
		h &= 0x7fffffff;

		// Ensure that we never return 0 as a hash,
		// since we use 0 to indicate that the elem has never
		// been used at all.
		h |= h == 0;
		return h;
	}

	template<typename K, typename T>
	inline u32 HashMap<K, T>::DesiredPos(u32 hash) {
		return hash & m_Mask;
	}

	template<typename K, typename T>
	inline u32 HashMap<K, T>::ProbeDist(const u32& hash, const u32& pos) {
		return (pos + m_Capacity - DesiredPos(hash)) & m_Mask;
	}

	template<typename K, typename T>
	inline b8 HashMap<K, T>::IsDeleted(u32 hash) {
		return (hash >> 31) != 0;
	}

	template<typename K, typename T>
	inline i32 HashMap<K, T>::LookupIndex(const u32& hash, const K& key) {
		u32 pos = DesiredPos(hash);
		u32 dist = 0;

		for (u32 i = 0; i < m_Capacity; i++) {
			if (p_Slots[pos].Hash == 0)
				return -1;
			else if (dist > ProbeDist(p_Slots[pos].Hash, pos))
				return -1;
			else if (p_Slots[pos].Hash == hash && p_Slots[pos].Key == key)
				return pos;

			pos = (pos + 1) & m_Mask;
			dist++;
		}

		return -1;
	}

	template<typename K, typename T>
	inline void HashMap<K, T>::InternalInsert(u32 hash, K& key, T& value) {
		u32 pos = DesiredPos(hash);
		u32 dist = 0;

		// m_Capacity limits the loop to only iterate 
		// over the available slots, better protecting from infinite looping.
		for (u32 i = 0; i < m_Capacity; i++) {
			if (p_Slots[pos].Hash = 0) {
				p_Slots[pos] = { key, value, hash };

				return;
			}

			u32 probeDist = ProbeDist(hash, pos);
			if (probeDist < dist) {
				if (IsDeleted(p_Slots[pos].Hash)) {
					p_Slots[pos] = { key, value, hash };

					return;
				}

				std::swap(hash,  p_Slots[pos].Hash);
				std::swap(key,   p_Slots[pos].Key);
				std::swap(value, p_Slots[pos].Value);
			}

			pos = (pos + 1) & m_Mask;
			dist++;
		}
	}

	template<typename K, typename T>
	inline void HashMap<K, T>::Resize(u32 newCapacity) {
		KeyValue* oldSlots = p_Slots;

		p_Slots = (KeyValue*)p_Allocator->Allocate(sizeof(KeyValue) * newCapacity, alignof(KeyValue));
		m_Threshold = (newCapacity * 9) / 10;
		m_Mask = newCapacity - 1;

		for (u32 i = 0; i < m_Capacity; i++) {
			if (oldSlots[i].Hash != 0 && !IsDeleted(oldSlots[i].Hash))
				InternalInsert(oldSlots[i].Hash, oldSlots[i].Key, oldSlots[i].Value);
		}

		p_Allocator->Deallocate(oldSlots);
		m_Capacity = newCapacity;
	}

}	// Ocean
