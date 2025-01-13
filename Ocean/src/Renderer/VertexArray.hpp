#pragma once

#include "Ocean/Core/Types/SmartPtrs.hpp"

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

            virtual void AddVertexBuffer(const Ref<VertexBuffer>& buffer) = 0;
            virtual const DynamicArray<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;

            virtual void SetIndexBuffer(const Ref<IndexBuffer>& buffer) = 0;
            virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

            static Ref<VertexArray> Create();

        };  // VertexArray

    }   // Shrimp

}   // Ocean
