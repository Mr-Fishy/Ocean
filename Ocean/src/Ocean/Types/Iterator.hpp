#pragma once

#include "Ocean/Types/Bool.hpp"

// std
#include <cstddef>
#include <iterator>

// See https://en.cppreference.com/w/cpp/iterator for more information.

/**
 * @brief An input iterator allows reading data from a sequence.
 * 
 * @tparam T The data type.
 */
template <class T>
class InputIterator {
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    InputIterator(pointer ptr) :
        p_Ptr(ptr)
    { }

    reference operator * () {
        return *this->p_Ptr;
    }

    InputIterator& operator ++ () {
        ++this->p_Ptr;

        return *this;
    }

    b8 operator == (const InputIterator& other) const {
        return this->p_Ptr == other.p_Ptr;
    }
    b8 operator != (const InputIterator& other) const {
        return this->p_Ptr != other.p_Ptr;
    }

protected:
    pointer p_Ptr;

};  // InputIterator

/**
 * @brief An output iterator allows writing data to a sequence.
 * 
 * @tparam T The data type.
 */
template <class T>
class OutputIterator {
public:
    using iterator_category = std::output_iterator_tag;
    using value_type = T;
    using difference_type = void;
    using pointer = T*;
    using reference = T&;

    OutputIterator(pointer ptr) :
        p_Ptr(ptr)
    { }

    OutputIterator& operator = (value_type value) {
        *this->p_Ptr = value;

        return *this;
    }

    OutputIterator& operator ++ () {
        return *this;
    }
    OutputIterator& operator + (difference_type) {
        return *this;
    }

protected:
    pointer p_Ptr;

};  // OutputIterator

/**
 * @brief A forward iterator allows reading and writing data, and can be incremented.
 * 
 * @tparam T The data type.
 */
template <class T>
class ForwardIterator : public InputIterator<T> {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    ForwardIterator(pointer ptr) :
        InputIterator<T>(ptr)
    { }

};  // ForwardIterator

/**
 * @brief A bidirectional iterator allows moving both forward and backward.
 * 
 * @tparam T The data type.
 */
template <class T>
class BidirectionalIterator : public ForwardIterator<T> {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    BidirectionalIterator(pointer ptr) :
        ForwardIterator<T>(ptr)
    { }

    BidirectionalIterator& operator -- () {
        --this->p_Ptr;

        return *this;
    }

};  // BidirectionalIterator

/**
 * @brief A random access iterator allows direct access to any element in the sequence.
 * 
 * @tparam T The data type.
 */
template <class T>
class RandomAccessIterator : public BidirectionalIterator<T> {
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    RandomAccessIterator(pointer ptr) :
        BidirectionalIterator<T>(ptr)
    { }

    RandomAccessIterator operator + (difference_type n) const {
        return RandomAccessIterator<T>(this->p_Ptr + n);
    }
    RandomAccessIterator operator - (difference_type n) const {
        return RandomAccessIterator<T>(this->p_Ptr - n);
    }

    difference_type operator - (const RandomAccessIterator<T>& rai) {
        return this->p_Ptr - rai.p_Ptr;
    }

};  // RandomAccessIterator
