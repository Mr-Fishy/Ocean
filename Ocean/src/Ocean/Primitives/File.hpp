#pragma once

#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/Strings.hpp"

#include "Ocean/Primitives/Macros.hpp"

namespace Ocean {

    /**
     * @brief A simple FILE wrapper that adds simplistic safety and stores the file data.
     */
    class File {
    public:
        File();
        /**
         * @brief Construct a new File object.
         * 
         * @param source The source file to open.
         */
        File(cstring source);
        ~File();
        
        File& operator = (const File &) = default;
        OC_NO_MOVE(File);

        /**
         * @brief Get the size of the file data.
         * 
         * @return sizet 
         */
        OC_INLINE sizet Size() const { return this->m_Size; }
        /**
         * @brief Get the file data.
         * 
         * @return u8* 
         */
        OC_INLINE u8* Data() const { return this->p_Data; }

    private:
        /** @brief The file data in memory. */
        u8* p_Data;
        /** @brief The size of the file data in memory. */
        sizet m_Size;

    };

}   // Ocean