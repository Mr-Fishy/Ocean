#include "Bitrix.hpp"

#include "Ocean/Types/Bix.hpp"
#include "Ocean/Types/Integers.hpp"

#include "Ocean/Primitives/Memory.hpp"

// std
#include <cstring>

Bitrix2D::Bitrix2D() :
    m_Width(0),
    m_VirtHeight(0),
    m_RealHeight(0),
    p_Bits(nullptr)
{ }

Bitrix2D::Bitrix2D(u16 width) :
    m_Width(0),
    m_VirtHeight(0),
    m_RealHeight(0),
    p_Bits(nullptr)
{
    Resize(width, width, false);
}

Bitrix2D::Bitrix2D(u16 width, u16 height) :
    m_Width(0),
    m_VirtHeight(0),
    m_RealHeight(0),
    p_Bits(nullptr)
{
    Resize(width, height, false);
}

Bitrix2D::~Bitrix2D()
{
    if (this->p_Bits != nullptr)
        ofree(this->p_Bits, oSystemAllocator);
}

void Bitrix2D::Set(u16 x, u16 y, b8 value) {
    this->p_Bits[x][y / 8].Set(y & ~(7), value);
}

b8 Bitrix2D::Get(u16 x, u16 y) {
    return this->p_Bits[x][y / 8][y & ~(7)];
}

void Bitrix2D::Reserve(u16 width, b8 value) {
    if (this->m_Width < width || this->m_VirtHeight < width)
        Resize(width, width, value);
}

void Bitrix2D::Reserve(u16 width, u16 height, b8 value) {
    if (this->m_Width < width || this->m_VirtHeight < height)
        Resize(width, height, value);
}

std::ostream& operator << (std::ostream& os, const Bitrix2D& rhs) {
    for (u16 i = 0; i < rhs.m_RealHeight; i++) {
        for (u16 k = 0; k < rhs.m_Width; k++)
            os << rhs.p_Bits[i][k];

        os << "\n";
    }

    return os;
}

void Bitrix2D::Resize(u16 width, u16 height, b8 value) {
    Bix8** bixArray = oallocat(Bix8*, width, oSystemAllocator);
    u16 newheight = (height & ~(7)) == 0 ? height / k_BixSize : height / k_BixSize + 1;

    for (u16 i = 0; i < width; i++) {
        bixArray[i] = oallocat(Bix8, newheight, oSystemAllocator);
        bixArray[i] = new(bixArray[i]) Bix8(value);

        if (i < this->m_Width && newheight >= this->m_RealHeight) {
            for (u16 k = 0; k < this->m_RealHeight; k++)
                bixArray[i][k] = this->p_Bits[i][k];
        }
    }

    ofree(this->p_Bits, oSystemAllocator);
    this->p_Bits = bixArray;

    this->m_Width = width;
    this->m_VirtHeight = height;
    this->m_RealHeight = newheight;
}
