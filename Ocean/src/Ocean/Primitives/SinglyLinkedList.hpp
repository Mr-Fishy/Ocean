#pragma once

#include "Ocean/Primitives/Exceptions.hpp"
#include "Ocean/Primitives/Memory.hpp"

#include "Ocean/Primitives/Abstract/List.hpp"

// std
#include <algorithm>

/**
 * @brief A Singly Linked List that stores data via one-directionaly connected nodes.
 * 
 * @tparam T The data type.
 */
template <class T>
class SinglyLinkedList : public List<T> {
private:
    /**
     * @brief A data node within the Singly Linked List.
     */
    struct Node {
        /** @brief A pointer to the next Node. */
        Node* next;

        /** @brief The stored data of type T. */
        T data;

    };  // Node

public:
    SinglyLinkedList() :
        List<T>(),
        p_Head(nullptr)
    { }

    /**
     * @brief Insert the given data into the Singly Linked List at a position via move.
     * 
     * @param pos The position to insert to.
     * @param data The data to store.
     */
    virtual void Insert(u16 pos, const T& data) {
        if (pos > this->m_Size)
            throw Ocean::Exception(Ocean::Error::OUT_OF_RANGE, "Attempt to insert out of List range!");

        Node* curr = this->p_Head;
        for (u16 i = 0; i < pos; i++)
            curr = curr->next;

        if (curr->next) {
            Node* tmp = curr->next;

            curr->next = oallocat(Node, 1, oUnmanagedAllocator);
            curr->next->next = tmp;
        }
        else {
            curr->next = oallocat(Node, 1, oUnmanagedAllocator);
        }

        curr->next->data = std::move(data);
        this->m_Size++;
    }

    /**
     * @brief Deconstructs and clears the Singly Linked List.
     * 
     * @note Does not handle pointer data.
     */
    virtual void Clear() {
        Node* curr = this->p_Head;

        for (u16 i = 0; i < this->m_Size; i++) {
            Node* tmp = curr;
            curr = curr->next;

            tmp->data.~T();
            ofree(tmp, oUnmanagedAllocator);
        }
    }

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
    virtual void Merge(List<T>& other) = 0;
    /**
     * @brief Removes all duplicate nodes within the List to make all nodes unique.
     */
    virtual void MakeUnique() = 0;
    /**
     * @brief Reverses the List order.
     */
    virtual void Reverse() = 0;

protected:
    Node* p_Head;

};  // SinglyLinkedList
