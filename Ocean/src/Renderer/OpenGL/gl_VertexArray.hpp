#pragma once

#include "Ocean/Core/Types/SharedPtr.hpp"
#include "Renderer/IndexBuffer.hpp"
#include "Renderer/VertexArray.hpp"
#include "Renderer/VertexBuffer.hpp"

namespace Ocean {

    namespace Shrimp {

        class glVertexArray : public VertexArray {
        public:
            glVertexArray();
            ~glVertexArray();

            virtual void Bind() const override final;
            virtual void Unbind() const override final;

            virtual void AddVertexBuffer(const SharedPtr<VertexBuffer>& buffer) override final;
            inline virtual const DynamicArray<SharedPtr<VertexBuffer>>& GetVertexBuffers() const override final { return m_VertexBuffers; }

            virtual void SetIndexBuffer(const SharedPtr<IndexBuffer>& buffer) override final;
            inline virtual const SharedPtr<IndexBuffer>& GetIndexBuffer() const override final { return m_IndexBuffer; }

        private:
            u32 m_RendererID;

            u32 m_VertexBufferIndex;

            DynamicArray<SharedPtr<VertexBuffer>> m_VertexBuffers;
            SharedPtr<IndexBuffer> m_IndexBuffer;

        };  // VertexArray

    }   // Shrimp

}   // Ocean
