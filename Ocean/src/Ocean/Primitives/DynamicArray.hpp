#pragma once

#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/Iterator.hpp"

#include "Ocean/Primitives/Memory.hpp"
#include "Ocean/Primitives/Exceptions.hpp"

#include "Ocean/Primitives/Abstract/Container.hpp"

// std
#include <initializer_list>
#include <algorithm>
#include <cstring>
#include <vector>

/**
 * @brief A dynamically sized array.
 * 
 * @tparam T The data type.
 */
template <class T>
class DynamicArray : public Container {
public:
    using Iterator = RandomAccessIterator<T>;
    using ConstIterator = RandomAccessIterator<const T>;

public:
    inline DynamicArray() :
        m_Size(0),
        m_Capacity(0),
        p_Data(nullptr)
    { }
    /**
     * @brief Construct a new Dynamic Array with a initial capacity of the given size.
     * 
     * @param size The initial size of the Dynamic Array.
     */
    inline DynamicArray(u16 size) :
        m_Size(0),
        m_Capacity(size),
        p_Data(oallocat(T, this->m_Capacity, oUnmanagedAllocator))
    { }
    /**
     * @brief Construct a new Dynamic Array from another Dynamic Array.
     * 
     * @param rhs The Dynamic Array to copy from.
     */
    inline DynamicArray(const DynamicArray& rhs) :
        m_Size(rhs.m_Size),
        m_Capacity(rhs.m_Capacity),
        p_Data(oallocat(T, this->m_Capacity, oUnmanagedAllocator))
    {
        for (u16 i = 0; i < this->m_Size; i++)
            this->p_Data[i] = rhs.p_Data[i];
    }
    /**
     * @brief Construct a new Dynamic Array from a std::vector.
     * 
     * @param rhs The std::vector to copy from.
     */
    inline DynamicArray(const std::vector<T>& rhs) :
        m_Size(rhs.size()),
        m_Capacity(rhs.capacity()),
        p_Data(oallocat(T, this->m_Capacity, oUnmanagedAllocator))
    {
        for (u16 i = 0; i < this->m_Size; i++)
            this->p_Data[i] = rhs[i];
    }
    /**
     * @brief Move a Dynamic Array to a new Dynamic Array.
     * 
     * @param other The Dynamic Array to move data from.
     */
    inline DynamicArray(DynamicArray&& other) :
        m_Size(other.m_Size),
        m_Capacity(other.m_Capacity),
        p_Data(other.p_Data)
    {
        other.m_Size = 0;
        other.m_Capacity = 0;
        other.p_Data = nullptr;
    }
    /**
     * @brief Construct a new Dynamic Array from an initial list.
     * 
     * @param list The initial list of type T to store.
     */
    inline DynamicArray(const std::initializer_list<T> &list) :
        m_Size(list.size()),
        m_Capacity(list.size()),
        p_Data(oallocat(T, this->m_Capacity, oUnmanagedAllocator))
    {
        // Because list will be destroyed we can utilize std::move.
        // 
        for (u16 i = 0; i < list.size(); i++)
            this->p_Data[i] = std::move(*(list.begin() + i));
    }
    inline ~DynamicArray() {
        if (this->p_Data)
            ofree(this->p_Data, oUnmanagedAllocator);
    }

    inline DynamicArray& operator = (const DynamicArray& rhs) {
        if (this != &rhs) {
            this->m_Size = rhs.m_Size;
            this->m_Capacity = rhs.m_Capacity;

            if (!this->p_Data)
                this->p_Data = oallocat(T, this->m_Capacity, oUnmanagedAllocator);

            for (u16 i = 0; i < this->m_Size; i++)
                this->p_Data[i] = rhs.p_Data[i];
        }

        return *this;
    }
    inline DynamicArray& operator = (DynamicArray&& other) {
        if (this->p_Data)
            ofree(this->p_Data, oUnmanagedAllocator);

        this->m_Size = other.m_Size;
        this->p_Data = other.p_Data;

        other.m_Size = 0;
        other.p_Data = nullptr;

        return *this;
    }

    /**
     * @brief Equality comparison with a generic Container.
     * 
     * @param other A valid Container.
     * @return b8 - True if equal, False otherwise.
     */
    inline virtual b8 operator == (const Container& other) const override {
        const DynamicArray<T>* rhs = dynamic_cast<const DynamicArray<T>*>(&other);

        if (rhs == nullptr)
            return false;

        if (this->m_Size != rhs->m_Size)
            return false;

        return std::equal(this->Begin(), this->End(), rhs->Begin());
    }
    /**
     * @brief In-equality comparison with a generic Container.
     * 
     * @param other A valid Container.
     * @return b8 - True if unequal, False otherwise.
     */
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
    inline constexpr const T& At(u16 index) const {
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
    inline constexpr T& operator [] (u16 i) { return this->p_Data[i]; }
    /**
     * @brief Gets the element at the given index.
     * 
     * @param i The index to get in the Array.
     * @return const T& 
     */
    inline constexpr const T& operator [] (u16 i) const { return this->p_Data[i]; }

    /**
     * @brief Gets the first element in the Array.
     * 
     * @return T& 
     */
    inline constexpr T& Front() { return this->p_Data[0]; }
    /**
     * @brief Gets the first element in the Array.
     * 
     * @return const T& 
     */
    inline constexpr const T& Front() const { return this->p_Data[0]; }
    /**
     * @brief Gets the last element in the Array.
     * 
     * @return T& 
     */
    inline constexpr T& Back() { return this->p_Data[this->m_Size - 1]; }
    /**
     * @brief Gets the last element in the Array.
     * 
     * @return const T& 
     */
    inline constexpr const T& Back() const { return this->p_Data[this->m_Size - 1]; }

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
     * @brief Constructs a new object of type T at the given position.
     * 
     * @tparam Args 
     * @param pos The position to construct a new object at.
     * @param args The type T constructor arguments.
     */
    template <class ... Args>
    inline constexpr void Emplace(u16 pos, Args ... args) {
        if (this->m_Size == this->m_Capacity)
            Resize(this->m_Capacity * 2);

        // Shift elements to the right to make space for the new element.
        //
        for (u16 i = this->m_Size; i > pos; i--)
            this->p_Data[i] = std::move(this->p_Data[i - 1]);

        // Construct the new element in place.
        //
        new (&this->p_Data[pos]) T(std::forward<Args>(args)...);

        this->m_Size++;
    }
    /**
     * @brief Constructs a new object of type T at the end of the Dynamic Array.
     * 
     * @tparam Args 
     * @param args The type T constructor arguments.
     */
    template <class ... Args>
    inline constexpr void EmplaceBack(Args ... args) {
        if (this->m_Size == this->m_Capacity)
            Resize(this->m_Capacity * 2);

        // Construct the new element in place at the end.
        //
        new (&this->p_Data[this->m_Size]) T(std::forward<Args>(args)...);

        this->m_Size++;
    }

    /**
     * @brief Deconstructs the elements within the Dynamic Array.
     */
    inline constexpr void Clear() {
        for (u16 i = 0; i < this->m_Size; i++)
            this->p_Data[i].~T();

        this->m_Size = 0;
    }

    /**
     * @brief Deconstructs and removes the object at the given position.
     * 
     * @param pos The integer position of the object.
     */
    inline constexpr void Erase(u16 pos) {
        if (pos >= this->m_Size)
            throw Ocean::Exception(Ocean::Error::OUT_OF_RANGE, "Index out of Array range!");

        // Call the deconstructor at the position.
        //
        this->p_Data[pos].~T();

        // Shift elements to the left to fill the space.
        //
        for (u16 i = pos; i < this->m_Size - 1; i++)
            this->p_Data[i] = std::move(this->p_Data[i + 1]);

        this->m_Size--;
    }
    /**
     * @brief Deconstructs and removes the object at the given position.
     * 
     * @param pos The Iterator position of the object.
     */
    inline constexpr void Erase(Iterator pos) {
        // Convert iterator to index and call the index erase.
        //
        Erase(static_cast<u16>(pos - Begin()));
    }
    /**
     * @brief Deconstructs and removes the object at the given position.
     * 
     * @param pos The ConstIterator position of the object.
     */
    inline constexpr void Erase(ConstIterator pos) {
        // Convert iterator to index and call the index erase.
        //
        Erase(static_cast<u16>(pos - Begin()));
    }
    /**
     * @brief Deconstructs and removes the objects within a given range.
     * 
     * @param first The first integer position in the range.
     * @param last The last integer position in the range.
     * @return u16 - The number of objects erased.
     */
    inline constexpr u16 Erase(u16 first, u16 last) {
        if (first >= this->m_Size || last > this->m_Size || first >= last)
            throw Ocean::Exception(Ocean::Error::OUT_OF_RANGE, "Invalid range for Erase!");

        u16 count = last - first;

        // Shift elements to the left to fill the space.
        //
        for (u16 i = last; i < this->m_Size; i++) {
            // Call the deconstructor.
            //
            this->p_Data[first].~T();

            this->p_Data[first++] = std::move(this->p_Data[i]);
        }

        this->m_Size -= count;
        return count;
    }
    /**
     * @brief Deconstructs and removes the objects within a given range.
     * 
     * @param first The first Iterator position in the range.
     * @param last The last Iterator position in the range.
     * @return u16 - The number of objects erased.
     */
    inline constexpr u16 Erase(Iterator first, Iterator last) {
        // Convert iterators to indices.
        //
        return Erase(static_cast<u16>(first - Begin()), static_cast<u16>(last - Begin()));
    }
    /**
     * @brief Deconstructs and removes the objects within a given range.
     * 
     * @param first The first ConstIterator position in the range.
     * @param last The last ConstIterator position in the range.
     * @return u16 - The number of objects erased.
     */
    inline constexpr u16 Erase(ConstIterator first, ConstIterator last) {
        // Convert iterators to indices.
        //
        return Erase(static_cast<u16>(first - Begin()), static_cast<u16>(last - Begin()));
    }

    /**
     * @brief Reserves the requested amount of space.
     * @note Reserve accounts for the number of existing items in the Dynamic Array. Making the total requested capacity = space + Size().
     * 
     * @param space The number of free elements to make space for.
     */
    inline constexpr void Reserve(u16 space) {
        if (this->m_Capacity < (space + this->m_Size))
            Resize(space + this->m_Size);
    }
    /**
     * @brief Resizes the Dynamic Array to the new size.
     * @note If there are active objects beyond the new size, then they will be deconstructed and erased.
     * 
     * @param newSize The new size of the Dynamic Array.
     */
    inline constexpr void Resize(u16 newSize) {
        if (newSize == 0)
            newSize = 2;
        // Deconstruct any objects beyond newSize if necessary.
        //
        if (newSize < this->m_Size) {
            for (u16 i = newSize; i < this->m_Size; i++)
                this->p_Data[i].~T();

            this->m_Size = newSize;
        }

        if (newSize == this->m_Capacity)
            return;

        T* newData = oallocat(T, newSize, oUnmanagedAllocator);
        
        for (u16 i = 0; i < this->m_Size; i++)
            newData[i] = std::move(this->p_Data[i]);

        if (this->p_Data)
            ofree(this->p_Data, oUnmanagedAllocator);

        this->p_Data = newData;
        this->m_Capacity = newSize;
    }

    /**
     * @brief Gets the internal data pointer.
     * 
     * @return T* 
     */
    inline constexpr T* Data() { return this->p_Data; }
    /**
     * @brief Gets the internal data pointer.
     * 
     * @return const T* 
     */
    inline constexpr const T* Data() const { return this->p_Data; }

    /**
     * @return b8 - True if the Array is empty, False otherwise.
     */
    inline constexpr b8 Empty() const { return this->m_Size == 0; }
    /**
     * @return u16 - The size of the Array.
     */
    inline constexpr u16 Size() const { return this->m_Size; }
    /**
     * @return u16 - The capacity of the Array.
     */
    inline constexpr u16 Capacity() const { return this->m_Capacity; }

    /**
     * @brief Gets a JSON useable format of the DynamicArray.
     * 
     * @param os 
     * @param rhs 
     * @return std::ostream& 
     */
    inline friend std::ostream& operator << (std::ostream& os, const DynamicArray<T>& rhs) {
        os << "{ ";

        for (u16 i = 0; i < (rhs.m_Size - 1); i++)
            os << rhs.p_Data[i] << ", ";

        os << rhs.p_Data[rhs.m_Size - 1] << " }";

        return os;
    }

protected:
    /** @brief The number of elements in the DynamicArray. */
    u16 m_Size;
    /** @brief The number of elements in memory of the DynamicArray. */
    u16 m_Capacity;
    /** @brief The data pointer of the DynamicArray. */
    T* p_Data;

};  // DynamicArray
