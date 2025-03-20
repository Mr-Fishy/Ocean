#pragma once

/**
 * @file List.hpp
 * @author Evan F.
 * @brief The header of the abstract List container.
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Types/Bool.hpp"
#include "Ocean/Types/Integers.hpp"

#include "Ocean/Primitives/Abstract/Container.hpp"

// std
#include <utility>

/**
 * @brief The abstract List container.
 * 
 * @tparam T The data type.
 */
template <class T>
class List : public Container {
public:
    inline List() :
        m_Size(0)
    { }
    /**
     * @brief Construct a new List with the given size.
     * 
     * @param size The initial size of the List.
     */
    inline List(u16 size) :
        m_Size(size)
    { }
    virtual ~List();

    /**
     * @brief Insert the given data into the List at a position via move.
     * 
     * @param pos The position to insert to.
     * @param data The data to store.
     */
    virtual void Insert(u16 pos, const T& data) = 0;

    /**
     * @brief Emplace an element into the List at a position via construction.
     * 
     * @tparam Args 
     * @param pos The position to emplace to.
     * @param args The constructor arguments.
     */
    template <class ... Args>
    void Emplace(u16 pos, Args&& ... args) {
        Insert(pos, T(std::forward(args)...));
    }
    /**
     * @brief Emplace an element into the List at the front via construction.
     * 
     * @tparam Args 
     * @param args The constructor arguments.
     */
    template <class ... Args>
    void EmplaceFront(Args&& ... args) {
        Insert(0, T(std::forward(args)...));
    }
    /**
     * @brief Emplace an element into the List at the back via construction.
     * 
     * @tparam Args 
     * @param args The constructor arguments.
     */
    template <class ... Args>
    void EmplaceBack(Args&& ... args) {
        Insert(this->m_Size, T(std::forward(args)...));
    }

    /**
     * @brief Clear's the List.
     * 
     * @note Does not handle pointer data.
     */
    virtual void Clear() = 0;

    /**
     * @brief Erases the node at the given position.
     * 
     * @param pos The position to erase.
     * 
     * @note Does not handle pointer data.
     */
    virtual void Erase(u16 pos) = 0;
    /**
     * @brief Removes the first encountered node with the given data.
     * 
     * @param data The data of the node to remove.
     * 
     * @note Does not handle pointer data.
     */
    virtual void Remove(const T& data) = 0;
    /**
     * @brief Removes all nodes with the given data.
     * 
     * @param data The data of the nodes to remove.
     * 
     * @note Does not handle pointer data.
     */
    virtual void RemoveAll(const T& data) = 0;

    /**
     * @brief Erases the first node in the List.
     * 
     * @note Does not handle pointer data.
     */
    virtual void PopFront() = 0;
    /**
     * @brief Erases the last node in the List.
     * 
     * @note Does not handle pointer data.
     */
    virtual void PopBack() = 0;

    /**
     * @brief Merges the List with the List given.
     * 
     * @param other The List to merge into this List.
     */
    virtual void Merge(List& other) = 0;
    /**
     * @brief Removes all duplicate nodes within the List to make all nodes unique.
     */
    virtual void MakeUnique() = 0;
    /**
     * @brief Reverses the List order.
     */
    virtual void Reverse() = 0;

    /**
     * @return b8 - True if the List is empty, False otherwise.
     */
    inline b8 Empty() const { return this->m_Size == 0; }
    /**
     * @return u16 - The size of the List.
     */
    inline u16 Size() const { return this->m_Size; }

protected:
    /** @brief The size of the List. */
    u16 m_Size;

};  // List
