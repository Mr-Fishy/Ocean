#pragma once

#include "Ocean/Types/Bool.hpp"
#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/Bix.hpp"

#include "Ocean/Primitives/Macros.hpp"

// std
#include <ostream>

/**
 * @brief A bit-compressed matrix, only holding true or false at a position.
 */
class Bitrix2D {
private:
    OC_STATIC_EXPR u8 k_BixSize = sizeof(Bix8) * 8;

public:
    Bitrix2D();
    /**
     * @brief Construct a new Bitrix2D object.
     * 
     * @param width The width to use for the matrix edges.
     */
    Bitrix2D(u16 width);
    /**
     * @brief Construct a new Bitrix2D object.
     * 
     * @param width The width of the matrix.
     * @param height The height of the matrix.
     */
    explicit Bitrix2D(u16 width, u16 height);
    virtual ~Bitrix2D();

    /**
     * @brief Set's the value of the given position.
     * 
     * @param x The x-coordinate of the position.
     * @param y The y-coordinate of the position.
     * @param value The value to set at the position.
     */
    virtual void Set(u16 x, u16 y, b8 value);
    /**
     * @brief Get's the value at the given position.
     * 
     * @param x The x-coordinate of the position.
     * @param y The y-coordinate of the position.
     * @return b8 
     */
    virtual b8 Get(u16 x, u16 y) const;

    /**
     * @brief Ensures that the matrix has enough space to fit the given edge width.
     * 
     * @param width The required width of the matrix edges.
     * @param value The value to set for any new columns / rows.
     */
    virtual void Reserve(u16 width, b8 value);
    /**
     * @brief Ensures that the matrix has enough space to fit the given width and height.
     * 
     * @param width The required width of the matrix.
     * @param height The required height of the matrix.
     * @param value The value to set for any new columns / rows.
     */
    virtual void Reserve(u16 width, u16 height, b8 value);

    /**
     * @brief Checks if the Bitrix is empty.
     * 
     * @return b8 
     */
    b8 Empty() const { return this->m_Width == 0; }

    u16 Width() const { return this->m_Width; }
    u16 Height() const { return this->m_VirtHeight; }

    /**
     * @brief Outputs the Bitrix2D to the ostream in a readable string format. 
     * 
     * @param os The ostream to output to.
     * @param rhs The Bitrix2D to output.
     * @return std::ostream& 
     */
    friend std::ostream& operator << (std::ostream& os, const Bitrix2D& rhs);

protected:
    /**
     * @brief Internal resize function to resize the matrix if necessary.
     * 
     * @param width The new width of the matrix.
     * @param height The new height of the matrix.
     * @param value The value to set for any new columns / rows.
     */
    virtual void Resize(u16 width, u16 height, b8 value);

    /** @brief The width of the matrix. This width corresponds to the number columns in a row. */
    u16 m_Width;
    /** @brief The virtual height of the matrix. This height corresponds to the number of bits in a column. */
    u16 m_VirtHeight;
    /** @brief The real height of the matrix. This height corresponds to the number of Bix8's in a column. */
    u16 m_RealHeight;

    /** @brief The bit-operable data as a 2D array. */
    Bix8** p_Bits;

};  // Bitrix2D
