#pragma once

#include "Ocean/Types/Bool.hpp"
#include "Ocean/Types/Integers.hpp"

// std
#include <initializer_list>
#include <ostream>

/**
 * @brief A bitwise wrapper for 8 bits.
 */
class Bix8 {
private:
    typedef u8 bitContainer;

public:
    Bix8();
    /**
     * @brief Construct a new 8-bit Bix object.
     * 
     * @param value The value to assign to the bits. I.e. a mask.
     */
    Bix8(bitContainer value);
    /**
     * @brief Construct a new 8-bit Bix object. Given a list of booleans from left to right.
     * 
     * @param list An initializer list of booleans to represent the bits. Must not exceed a length of 8.
     */
    Bix8(const std::initializer_list<b8>& list);
    ~Bix8();

    /**
     * @return b8 - True if this is equal to other, False otherwise.
     */
    b8 operator == (const Bix8 &other) const;
    /**
     * @return b8 - True if this is not equal to other, False otherwise.
     */
    b8 operator != (const Bix8 &other) const;

    /**
     * @param index The index of the bit, from left to right starting at 0.
     * @return b8 - True if the bit is 1, False if it is 0.
     */
    b8 operator [] (u8 index) const;

    /**
     * @brief Operates the same as index [] access with range access safety.
     * 
     * @param index The index of the bit, from left to right starting at 0.
     * @return b8 - True if the bit is 1, False if it is 0.
     */
    b8 At(u8 index) const;

    /**
     * @brief Flips the bit at the given index.
     * 
     * @param index The index of the bit, from left to right starting at 0.
     */
    void Flip(u8 index);
    /**
     * @brief Set's the bit at the given index.
     * 
     * @param index The index of the bit value to get, from left to right starting at 0.
     * @param value The value to set the bit. True == 1, False == 0.
     */
    void Set(u8 index, b8 value);
    /**
     * @brief Masks the internal value with the given mask.
     * 
     * @param mask A bitwise mask for a u8 value.
     */
    void Mask(bitContainer mask);

    /**
     * @brief Get's a copy of the internal value.
     * 
     * @return u8 
     */
    bitContainer Raw() const { return this->m_Val; }

    /**
     * @brief Clear's the internal bits.
     */
    void Clear() { this->m_Val = 0; }

    /**
     * @brief Outputs the Bix8 to the ostream in a readable string format.
     * 
     * @param os The ostream to output to.
     * @param rhs The Bix8 to output.
     * @return std::ostream& 
     */
    friend std::ostream &operator << (std::ostream &os, const Bix8 &rhs);

private:
    u8 Pos(u8 index) const { return (sizeof(bitContainer) * 8 - index - 1); }

private:
    /** @brief The internal 8-bit value. */
    bitContainer m_Val;

};  // Bix8
