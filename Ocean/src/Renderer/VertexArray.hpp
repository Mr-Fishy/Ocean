#pragma once

#include "Ocean/Core/Types/SharedPtr.hpp"

#include "Ocean/Core/Primitives/Array.hpp"

#include "Renderer/VertexBuffer.hpp"
#include "Renderer/IndexBuffer.hpp"

namespace Ocean {

    namespace Shrimp {

        class VertexArray {
        public:
            virtual ~VertexArray() = default;

            virtual void Bind() const = 0;
            virtual void Unbind() const = 0;

            virtual void AddVertexBuffer(const SharedPtr<VertexBuffer>& buffer) = 0;
            virtual const DynamicArray<SharedPtr<VertexBuffer>>& GetVertexBuffers() const = 0;

            virtual void SetIndexBuffer(const SharedPtr<IndexBuffer>& buffer) = 0;
            virtual const SharedPtr<IndexBuffer>& GetIndexBuffer() const = 0;

            static SharedPtr<VertexArray> Create();

        };  // VertexArray

    }   // Shrimp

}   // Ocean
