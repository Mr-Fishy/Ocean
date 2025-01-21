#pragma once

#include "Ocean/Types/Bool.hpp"
#include "Ocean/Types/Integers.hpp"

#include "Ocean/Primitives/Assert.hpp"
#include "Ocean/Primitives/Macros.hpp"
#include "Ocean/Primitives/Memory.hpp"

#include <cstddef>
#include <cstring>
#include <array>
#include <iterator>

template <class T>
class DynamicArray {
public:
    class Iterator : public std::iterator<std::input_iterator_tag, T, ptrdiff_t, T*, T&> {
    public:
        OC_INLINE Iterator(T* ptr) : p_Ptr(ptr) { }
        ~Iterator() = default;

        OC_INLINE T& operator * () { return *this->p_Ptr; }
        OC_INLINE T* operator -> () { return this->p_Ptr; }

        OC_INLINE Iterator& operator ++ () {
            ++p_Ptr; return *this;
        }
        OC_INLINE Iterator operator ++ (i32) {
            Iterator result(*this);
            return ++result;
        }

        OC_INLINE Iterator& operator += (u32 dist) {
            this->p_Ptr += dist;

            return *this;
        }
        OC_INLINE Iterator operator + (u32 dist) {
            Iterator result(this->p_Ptr);
            result += dist;
            return result;
        }

        OC_INLINE b8 operator == (const Iterator& other) const {
            return this->p_Ptr == other.p_Ptr;
        }
        OC_INLINE b8 operator != (const Iterator& other) const {
            return this->p_Ptr != other.p_Ptr;
        }

    private:
        T* p_Ptr; /** @brief Pointer to the current element of the Iterator. */

    };  // Iterator

    class ConstIterator : public std::iterator<std::input_iterator_tag, T, ptrdiff_t, T*, T&> {
    public:
        OC_INLINE ConstIterator(const T* ptr) : p_Ptr(ptr) { }
        ~ConstIterator() = default;

        OC_INLINE const T& operator * () const { return *this->p_Ptr; }

        OC_INLINE ConstIterator& operator ++ () {
            ++p_Ptr; return *this;
        }
        OC_INLINE ConstIterator operator ++ (i32) {
            ConstIterator result(*this);
            return ++result;
        }

        OC_INLINE b8 operator == (const ConstIterator& other) const {
            return this->p_Ptr == other.p_Ptr;
        }
        OC_INLINE b8 operator != (const ConstIterator& other) const {
            return this->p_Ptr != other.p_Ptr;
        }

    private:
        const T* p_Ptr; /** @brief Pointer to the current element of the ConstIterator. */

    };  // ConstIterator

public:
    OC_INLINE_EXPR DynamicArray() : p_Data(nullptr), m_Capacity(0), m_Size(0) { }
    OC_INLINE_EXPR DynamicArray(const DynamicArray<T>& other) : p_Data(nullptr), m_Capacity(other.m_Capacity), m_Size(other.m_Size) {
        this->p_Data = static_cast<T*>(oSystemAllocator->Allocate(sizeof(T) * this->m_Capacity, alignof(T)));

        std::memcpy(this->p_Data, other.p_Data, sizeof(T) * this->m_Size);
    }
    OC_INLINE_EXPR DynamicArray(const std::initializer_list<T>& list) : p_Data(nullptr), m_Capacity(list.size()), m_Size(list.size()) {
        this->p_Data = static_cast<T*>(oSystemAllocator->Allocate(sizeof(T) * this->m_Capacity, alignof(T)));

        for (u32 i = 0; i < this->m_Size; i++)
            this->p_Data[i] = std::move(*(list.begin() + i));
    }
    OC_INLINE ~DynamicArray() {
        if (this->m_Size != 0) {
            oSystemAllocator->Deallocate(this->p_Data);
        }
    }

    OC_INLINE_EXPR DynamicArray<T>& operator = (const DynamicArray<T>& other) {
        if (&other != this) {
            if (this->m_Size != 0)
                oSystemAllocator->Deallocate(this->p_Data);

            this->m_Capacity = other.m_Capacity;
            this->m_Size = other.m_Size;

            this->p_Data = static_cast<T*>(oSystemAllocator->Allocate(sizeof(T) * this->m_Capacity, alignof(T)));

            std::memcpy(this->p_Data, other.p_Data, sizeof(T) * this->m_Size);
        }

        return *this;
    }

    OC_INLINE_EXPR void PushBack(const T& value) {
        if (this->m_Size >= this->m_Capacity)
            Grow(this->m_Capacity == 0 ? 1 : this->m_Capacity * 2);

        this->p_Data[this->m_Size++] = value;
    }
    OC_INLINE_EXPR void PopBack() {
        OASSERTM(this->m_Size > 0, "Array is already empty!");

        this->m_Size--;
    }

    OC_INLINE_EXPR void Emplace(u32 index, T&& value) {
        if (this->m_Size >= this->m_Capacity)
            Grow(this->m_Capacity == 0 ? 1 : this->m_Capacity * 2);

        for (u32 i = this->m_Size; i > index; i--)
            this->p_Data[i] = std::move(this->p_Data[i - 1]);

        new(&this->p_Data[index]) T(std::forward<T>(value));
        ++this->m_Size;
    }
    template <typename ... Args>
    OC_INLINE_EXPR void Emplace(u32 index, Args&& ... args) {
        if (this->m_Size >= this->m_Capacity)
            Grow(this->m_Capacity == 0 ? 1 : this->m_Capacity * 2);

        for (u32 i = this->m_Size; i > index; i--)
            this->p_Data[i] = std::move(this->p_Data[i - 1]);

        new(&this->p_Data[index]) T(std::forward<Args>(args)...);
        ++this->m_Size;
    }
    OC_INLINE_EXPR void EmplaceBack(T&& value) {
        if (this->m_Size >= this->m_Capacity)
            Grow(this->m_Capacity == 0 ? 1 : this->m_Capacity * 2);

        new(&this->p_Data[++this->m_Size]) T(std::forward<T>(value));
    }
    template <typename ... Args>
    OC_INLINE_EXPR void EmplaceBack(Args&& ... args) {
        if (this->m_Size >= this->m_Capacity)
            Grow(this->m_Capacity == 0 ? 1 : this->m_Capacity * 2);

        new(&this->p_Data[++this->m_Size]) T(std::forward<Args>(args)...);
    }

    OC_INLINE_EXPR void Erase(u32 index) {
        OASSERT_LENGTH(index, this->m_Size);

        this->m_Size--;
        for (u32 i = index; i < this->m_Size; i++)
            this->p_Data[i] = std::move(this->p_Data[i + 1]);
    }

    OC_INLINE_EXPR T& At(u32 index) { return this[index]; }
    OC_INLINE_EXPR const T& At(u32 index) const { return this[index]; }

    OC_INLINE_EXPR T& operator [] (u32 index) {
        OASSERT_LENGTH(index, this->m_Size);

        return this->p_Data[index];
    }
    OC_INLINE_EXPR const T& operator [] (u32 index) const {
        OASSERT_LENGTH(index, this->m_Size);

        return this->p_Data[index];
    }

    OC_INLINE_EXPR T& Front() { return this->p_Data; }
    OC_INLINE_EXPR const T& Front() const { return this->p_Data; }

    OC_INLINE_EXPR T& Back() { return this->p_Data + this->m_Size; }
    OC_INLINE_EXPR const T& Back() const { return this->p_Data + this->m_Size; }

    OC_INLINE Iterator begin() { return Iterator(this->p_Data); }
    OC_INLINE const ConstIterator begin() const { return ConstIterator(this->p_Data); }

    OC_INLINE Iterator end() { return Iterator(this->p_Data + this->m_Size); }
    OC_INLINE const ConstIterator end() const { return ConstIterator(this->p_Data + this->m_Size); }

    OC_INLINE b8 Empty() const { return this->m_Size == 0; }
    OC_INLINE u32 Size() const { return this->m_Size; }
    OC_INLINE u32 Capacity() const { return this->m_Capacity; }

    OC_INLINE_EXPR T* Data() const { return this->p_Data; }

    OC_INLINE void Clear() { this->m_Size = 0; }

    OC_INLINE void Resize(u32 newCapacity) {
        T* newData = static_cast<T*>(oSystemAllocator->Allocate(sizeof(T) * newCapacity, alignof(T)));

        std::memcpy(newData, this->p_Data, sizeof(T) * (this->m_Size > newCapacity ? newCapacity : this->m_Size));

        oSystemAllocator->Deallocate(this->p_Data);

        this->p_Data = newData;
        this->m_Capacity = newCapacity;
    }

private:
    OC_INLINE_EXPR void Grow(u32 newCapacity) {
        T* newData = static_cast<T*>(oSystemAllocator->Allocate(sizeof(T) * newCapacity, alignof(T)));

        for (u32 i = 0; i < this->m_Size; i++)
            newData[i] = std::move(this->p_Data[i]);

        oSystemAllocator->Deallocate(this->p_Data);
        
        this->p_Data = newData;
        this->m_Capacity = newCapacity;
    }

    /* --- */

    T* p_Data;

    u32 m_Capacity;
    u32 m_Size;

};  // DynamicArray

template <class T, sizet S>
using FixedArray = std::array<T, S>;
