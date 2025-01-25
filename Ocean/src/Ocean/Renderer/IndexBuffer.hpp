#pragma once

/**
 * @file IndexBuffer.hpp
 * @author Evan F.
 * @brief The abstract IndexBuffer for Ocean.
 * @date 01-14-2025
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Types/SmartPtrs.hpp"
#include "Ocean/Types/Integers.hpp"

#include "Ocean/Primitives/Macros.hpp"

namespace Ocean {

    namespace Shrimp {

        /**
         * @brief The IndexBuffer controls an array of indices through the renderer API.
         */
        class IndexBuffer {
        public:
            virtual ~IndexBuffer() = default;

            /**
             * @brief Bind's the IndexBuffer to be used by commmands.
             */
            virtual void Bind() const = 0;
            /**
             * @brief Unbind's the IndexBuffer from being able to be used by commands.
             */
            virtual void Unbind() const = 0;

            /**
             * @brief Get the number of indices in the buffer.
             * 
             * @return u32 
             */
            virtual u32 GetCount() const = 0;

            /**
             * @brief Create's a new VertexArray with the given indices.
             * 
             * @param indices The array of indices to use.
             * @param count The number of indices.
             * @return Ref<IndexBuffer> 
             */
            OC_STATIC Ref<IndexBuffer> Create(u32* indices, u32 count);

        };  // IndexBuffer

    }   // Shrimp

}   // Ocean
