#pragma once

#include "Ocean/Types/Bool.hpp"
#include "Ocean/Types/Strings.hpp"

#include "Ocean/Primitives/Macros.hpp"

// std
#include <ostream>

// May be of use: https://archive.org/details/optimized-c/page/336/mode/2up

/**
 * @brief A abstract Container class defining the requirements of all Ocean Containers.
 */
class Container {
public:
    virtual ~Container() = default;

    virtual b8 operator == (const Container& other) const {
        if (typeid(*this) != typeid(other))
            return false;

        return true;
    }
    virtual b8 operator != (const Container& other) const {
        return !(*this == other);
    }

    /**
     * @brief Gets a JSON useable format of the Container.
     * 
     * @param os 
     * @param rhs 
     * @return std::ostream& 
     */
    friend std::ostream& operator << (std::ostream& os, const Container& rhs) {
        os << &rhs;

        return os;
    }

};  // Container
