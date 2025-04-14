#pragma once

#include "Ocean/Types/Bool.hpp"
#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/Iterator.hpp"
#include "Ocean/Types/Strings.hpp"

#include "Ocean/Primitives/Memory.hpp"
#include "Ocean/Primitives/Exceptions.hpp"

#include "Ocean/Primitives/Structures/Container.hpp"

// std
#include <initializer_list>
#include <ostream>

/**
 * @brief A fixed size array.
 * 
 * @tparam T The data type.
 * @tparam S The size of the array.
 */
template <class T, u16 S>
class FixedArray : public Container {
public:
    using Iterator = RandomAccessIterator<T>;
    using ConstIterator = RandomAccessIterator<const T>;

    public:
    FixedArray() :
        m_Size(0),
        p_Data(oallocat(T, S, oUnmanagedAllocator))
    { }
    FixedArray(const FixedArray& rhs) :
        m_Size(rhs.m_Size),
        p_Data(oallocat(T, S, oUnmanagedAllocator))
    {
        for (u16 i = 0; i < this->m_Size; i++)
            this->p_Data[i] = rhs.p_Data[i];
    }
    FixedArray(FixedArray&& other) :
        m_Size(other.m_Size),
        p_Data(other.p_Data)
    {
        other.m_Size = 0;
        other.p_Data = oallocat(T, S, oUnmanagedAllocator);
    }
    FixedArray(const std::initializer_list<T> &list) :
        m_Size(list.size()),
        p_Data(oallocat(T, S, oUnmanagedAllocator))
    {
      if (list.size() >= S)
        throw Ocean::Exception(Ocean::Error::OVERFLOW_ERROR, "Initializer list length greater than FixedArray capacity!");

      // Because list will be destroyed we can utilize std::move.
      // 
      for (u16 i = 0; i < list.size(); i++)
        this->p_Data[i] = std::move(*(list.begin() + i));
    }
    ~FixedArray() {
        if (this->p_Data)
            ofree(this->p_Data, oUnmanagedAllocator);
    }

    FixedArray& operator = (const FixedArray& rhs) {
        if (*this != rhs) {
            this->m_Size = rhs.m_Size;

            for (u16 i = 0; i < this->m_Size; i++)
                this->p_Data[i] = rhs.p_Data[i];
        }

        return *this;
    }
    FixedArray& operator = (FixedArray&& other) {
        if (this->p_Data)
            ofree(this->p_Data, oUnmanagedAllocator);

        this->m_Size = other.m_Size;
        this->p_Data = other.p_Data;

        other.m_Size = 0;
        other.p_Data = nullptr;

        return *this;
    }

    inline virtual b8 operator == (const Container& other) const override {
        const FixedArray<T, S>* rhs = dynamic_cast<const FixedArray<T, S>*>(&other);

        if (rhs == nullptr)
            return false;

        if (this->m_Size != rhs->m_Size)
            return false;

        return std::equal(this->Begin(), this->End(), rhs->Begin());
    }
    inline virtual b8 operator != (const Container& other) const override {
        return !(*this == other);
    }

    /**
     * @brief Gets the element at the given index with range checking.
     * 
     * @param index The index to get in the Array.
     * @return T& 
     */
    inline constexpr T& At(u16 index) {
        if (index >= this->m_Size)
            throw Ocean::Exception(Ocean::Error::OUT_OF_RANGE, "Index out of Array range!");

        return this->p_Data[index];
    }
    /**
     * @brief Gets the element at the given index with range checking.
     * 
     * @param index The index to get in the Array.
     * @return const T& 
     */
    constexpr const T& At(u16 index) const {
        if (index >= this->m_Size)
            throw Ocean::Exception(Ocean::Error::OUT_OF_RANGE, "Index out of Array range!");

        return this->p_Data[index];
    }

    /**
     * @brief Gets the element at the given index.
     * 
     * @param i The index to get in the Array.
     * @return T& 
     */
    constexpr T& operator [] (u16 i) { return this->p_Data[i]; }
    /**
     * @brief Gets the element at the given index.
     * 
     * @param i The index to get in the Array.
     * @return const T& 
     */
    constexpr const T& operator [] (u16 i) const { return this->p_Data[i]; }

    /**
     * @brief Gets the first element in the Array.
     * 
     * @return T& 
     */
    constexpr T& Front() { return this->p_Data[0]; }
    /**
     * @brief Gets the first element in the Array.
     * 
     * @return const T& 
     */
    constexpr const T& Front() const { return this->p_Data[0]; }
    /**
     * @brief Gets the last element in the Array.
     * 
     * @return T& 
     */
    constexpr T& Back() { return this->p_Data[this->m_Size - 1]; }
    /**
     * @brief Gets the last element in the Array.
     * 
     * @return const T& 
     */
    constexpr const T& Back() const { return this->p_Data[this->m_Size - 1]; }

    /**
     * @brief Gets a Iterator to the beginning of the DynamicArray.
     * 
     * @return Iterator 
     */
    inline constexpr Iterator Begin() { return Iterator(this->p_Data); }
    inline constexpr Iterator begin() { return Begin(); }
    /**
     * @brief Gets a ConstIterator to the beginning of the DynamicArray.
     * 
     * @return ConstIterator 
     */
    inline constexpr ConstIterator Begin() const { return ConstIterator(this->p_Data); }
    inline constexpr ConstIterator begin() const { return Begin(); }
    /**
     * @brief Gets a Iterator to the end of the DynamicArray.
     * 
     * @return Iterator 
     */
    inline constexpr Iterator End() { return Iterator(this->p_Data + this->m_Size); }
    inline constexpr Iterator end() { return End(); }
    /**
     * @brief Gets a ConstIterator to the end of the DynamicArray.
     * 
     * @return ConstIterator 
     */
    inline constexpr ConstIterator End() const { return ConstIterator(this->p_Data + this->m_Size); }
    inline constexpr ConstIterator end() const { return End(); }

    /**
     * @brief Gets the internal data pointer.
     * 
     * @return T* 
     */
    constexpr T* Data() { return this->p_Data; }
    /**
     * @brief Gets the internal data pointer.
     * 
     * @return const T* 
     */
    constexpr const T* Data() const { return this->p_Data; }

    /**
     * @return b8 - True if the Array is empty, False otherwise.
     */
    constexpr b8 Empty() const { return this->m_Size == 0; }
    /**
     * @return u16 - The size of the Array.
     */
    constexpr u16 Size() const { return this->m_Size; }
    /**
     * @return u16 - The capacity of the Array.
     */
    constexpr u16 Capacity() const { return S; }

    /**
     * @brief Gets a JSON useable format of the FixedArray.
     * 
     * @param os 
     * @param rhs 
     * @return std::ostream& 
     */
    friend std::ostream& operator << (std::ostream& os, const FixedArray<T, S>& rhs) {
        os << "{ ";

        for (u16 i = 0; i < (S - 1); i++)
            os << rhs[i] << ", ";

        os << rhs[S - 1] << " }";

        return os;
    }

protected:
    /** @brief The number of elements in the FixedArray. */
    u16 m_Size;
    /** @brief The data pointer of the FixedArray. */
    T* p_Data;

};  // FixedArray
