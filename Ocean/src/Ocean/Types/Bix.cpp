#include "Bix.hpp"

#include "Ocean/Primitives/Exceptions.hpp"

// std
#include <iostream>

Bix8::Bix8() :
    m_Val(0)
{ }

Bix8::Bix8(u8 value) :
    m_Val(value)
{ }

Bix8::Bix8(const std::initializer_list<b8>& list) :
    m_Val(0)
{
    if (list.size() > sizeof(bitContainer) * 8)
        throw Ocean::Exception(Ocean::Error::OVERFLOW_ERROR, "Given more items than type supports!");

    u8 i = 0;
    for (b8 val : list) {
        if (val)
            this->m_Val |= 1 << (7 - i);

        i++;
    }
}

Bix8::~Bix8()
{ }

b8 Bix8::operator == (const Bix8 &other) const {
    return this->m_Val & other.m_Val;
}

b8 Bix8::operator != (const Bix8 &other) const {
    return !(*this == other);
}

b8 Bix8::operator [] (bitContainer index) const {
    return this->m_Val & (1U << Pos(index));
}

b8 Bix8::At(u8 index) const {
    if (index >= sizeof(bitContainer) * 8)
        throw Ocean::Exception(Ocean::Error::OUT_OF_RANGE, "Attempt to access bit out of range!");

    return this->m_Val & (1 << Pos(index));
}

void Bix8::Flip(u8 index) {
    if (index >= sizeof(bitContainer) * 8)
        throw Ocean::Exception(Ocean::Error::OUT_OF_RANGE, "Attempt to access bit out of range!");

    this->m_Val ^= 1 << Pos(index);
}

void Bix8::Set(u8 index, b8 value) {
    if (index >= sizeof(bitContainer) * 8)
        throw Ocean::Exception(Ocean::Error::OUT_OF_RANGE, "Attempt to access bit out of range!");

    if (!value)
        this->m_Val &= ~(1 << Pos(index));
    else
        this->m_Val |= 1 << Pos(index);
}

void Bix8::Mask(bitContainer mask) {
    this->m_Val &= mask;
}

std::ostream &operator << (std::ostream &os, const Bix8 &rhs) {
    for (u8 i = 0; i < sizeof(Bix8::bitContainer) * 8; i++)
        os << rhs[i];

    return os;
}
