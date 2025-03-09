#pragma once

#include "Ocean/Types/Bool.hpp"
#include "Ocean/Types/Integers.hpp"

// std
#include <initializer_list>
#include <ostream>

/**
 * @brief A base Bix.
 */
class Bix {
public:
    virtual ~Bix();

    virtual b8 At(u8 index) const = 0;
    virtual void Set(u8 index, b8 value) = 0;
};


/**
 * @brief Accesses one bit in a Bix, enabling bool setting.
 */
class BixAccess {
    public:
        BixAccess(const BixAccess&) = default;
        BixAccess(BixAccess&&) = default;
        BixAccess& operator = (const BixAccess& rhs) = delete;
        BixAccess& operator = (BixAccess&&) = delete;

        BixAccess(Bix &bix, u8 index);

        /**
         * @brief Set's the position to the given val in the Bix.
         * 
         * @param val The val to set the bit.
         * @return BixAccess& 
         */
        BixAccess& operator = (b8 val) {
            this->m_Ref.Set(this->m_Index, val);

            return *this;
        }

    private:
        Bix& m_Ref;
        u8 m_Index;

    };  // BixAccess

/**
 * @brief A bitwise wrapper for 8 bits.
 */
class Bix8 : public Bix {
public:
    Bix8();
    /**
     * @brief Construct a new 8-bit Bix object.
     * 
     * @param signal The signal to assign to the bits. I.e. True for all 1's and False for all 0's
     */
    Bix8(b8 signal);
    /**
     * @brief Construct a new 8-bit Bix object.
     * 
     * @param value The value to assign to the bix.
     */
    Bix8(i32 value);
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
     * @return BixAccess 
     */
    BixAccess operator [] (u8 index);
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
    virtual b8 At(u8 index) const override;

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
    virtual void Set(u8 index, b8 value) override;
    /**
     * @brief Masks the internal value with the given mask.
     * 
     * @param mask A bitwise mask for a u8 value.
     */
    void Mask(u8 mask);

    /**
     * @brief Get's a copy of the internal value.
     * 
     * @return u8 
     */
    inline u8 Raw() const { return this->m_Val; }

    /**
     * @brief Clear's the internal bits.
     */
    inline void Clear(b8 value = false) { this->m_Val = (value ? u8_max : 0); }

    /**
     * @brief Outputs the Bix8 to the ostream in a readable string format.
     * 
     * @param os The ostream to output to.
     * @param rhs The Bix8 to output.
     * @return std::ostream& 
     */
    friend std::ostream &operator << (std::ostream &os, const Bix8 &rhs);

private:
    /**
     * @brief Convert's the left to right position to right to left position for bit operations.
     * 
     * @param index The left to right index.
     * @return u8 
     */
    u8 Pos(u8 index) const { return (sizeof(u8) * 8 - index - 1); }

private:
    /** @brief The internal 8-bit value. */
    u8 m_Val;

};  // Bix8
