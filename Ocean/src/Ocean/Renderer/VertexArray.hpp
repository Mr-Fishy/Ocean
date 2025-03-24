#pragma once

/**
 * @file VertexArray.hpp
 * @author Evan F.
 * @brief The abstract VertexArray for Ocean.
 * @date 01-14-2025
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/DynamicArray.hpp"

#include "Ocean/Renderer/VertexBuffer.hpp"
#include "Ocean/Renderer/IndexBuffer.hpp"

namespace Ocean {

    namespace Splash {

        /**
         * @brief The VertexArray controls an array of vertices through the renderer API.
         */
        class VertexArray {
        public:
            virtual ~VertexArray() = default;

            /**
             * @brief Bind's the VertexArray to be used by commmands.
             */
            virtual void Bind() const = 0;
            /**
             * @brief Unbind's the VertexArray from being able to be used by commands.
             */
            virtual void Unbind() const = 0;

            /**
             * @brief Add's a VertexBuffer to the VertexArray for rendering.
             * 
             * @param buffer The VertexBuffer to add.
             */
            virtual void AddVertexBuffer(const Ref<VertexBuffer>& buffer) = 0;
            /**
             * @brief Get the VertexBuffer objects assigned to the VertexArray.
             * 
             * @return const DynamicArray<Ref<VertexBuffer>>& 
             */
            virtual const DynamicArray<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;

            /**
             * @brief Set the IndexBuffer object to the given buffer.
             * 
             * @param buffer The IndexBuffer to use with the VertexBuffers.
             */
            virtual void SetIndexBuffer(const Ref<IndexBuffer>& buffer) = 0;
            /**
             * @brief Get the IndexBuffer object assigned to the VertexArray.
             * 
             * @return const Ref<IndexBuffer>& 
             */
            virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

            /**
             * @brief Create's a new VertexArray that controls the renderer API.
             * 
             * @return Ref<VertexArray> 
             */
            OC_STATIC Ref<VertexArray> Create();

        };  // VertexArray

    }   // Splash

}   // Ocean
