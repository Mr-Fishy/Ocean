#pragma once

#include "Ocean/Primitives/Exceptions.hpp"
#include "Ocean/Primitives/Memory.hpp"

#include "Ocean/Primitives/Strucutres/List.hpp"

// std
#include <algorithm>
#include <utility>

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

        Node* newNode = oallocat(Node, 1, oUnmanagedAllocator);

        if (pos == 0) {
            newNode->next = this->p_Head;
            this->p_Head = newNode;
        }
        else {
            Node* curr = this->p_Head;

            // Start at 1 so that the curr is the node before the insert position.
            //
            for (u16 i = 1; i < pos; i++)
                curr = curr->next;

            // If a node exists after the insert position, then it must be linked correctly.
            //
            if (curr->next) {
                newNode->next = curr->next;
                curr->next = newNode;
            }
            else {
                newNode->next = nullptr;
                curr->next = newNode;
            }
        }

        newNode->data = std::move(data);
        this->m_Size++;
    }

    /**
     * @brief Deconstructs and clears the Singly Linked List.
     * 
     * @note Does not handle pointer data.
     */
    virtual void Clear() {
        Node* next = this->p_Head;

        for (u16 i = 0; i < this->m_Size; i++) {
            Node* curr = next;
            next = next->next;

            curr->data.~T();
            ofree(curr, oUnmanagedAllocator);
        }

        this->p_Head = nullptr;
    }

    /**
     * @brief Deconstructs and removes the object at the given position.
     * 
     * @param pos The position to erase.
     * 
     * @note Does not handle pointer data.
     */
    virtual void Erase(u16 pos) {
        if (pos >= this->m_Size)
            throw Ocean::Exception(Ocean::Error::OUT_OF_RANGE, "Attempt to Erase List Node that does not exist!");

        Node* toDelete = this->p_Head;

        if (pos == 0) {
            this->p_Head = this->p_Head->next;
        }
        else {
            Node* curr = this->p_Head;

            // Start at 1 so that the curr is the node before the insert position.
            //
            for (u16 i = 1; i < pos; i++)
                curr = curr->next;

            toDelete = curr->next;
            curr->next = toDelete->next;
        }

        toDelete->data.~T();
        ofree(toDelete, oUnmanagedAllocator);
        this->m_Size--;
    }
    /**
     * @brief Deconstructs and removes the first encountered object with the given data.
     * 
     * @param data The data of the node to remove.
     * 
     * @note Does not handle pointer data.
     */
    virtual void Remove(const T& data) {
        Node* curr = this->p_Head;

        while (curr->next && curr->data != data)
            curr = curr->next;

        
    }
    /**
     * @brief Deconstructs and removes all objects with the given data.
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
