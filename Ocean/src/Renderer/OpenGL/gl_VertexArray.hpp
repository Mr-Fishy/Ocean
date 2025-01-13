#pragma once

#include "Ocean/Core/Types/SmartPtrs.hpp"

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

            virtual void AddVertexBuffer(const Ref<VertexBuffer>& buffer) override final;
            inline virtual const DynamicArray<Ref<VertexBuffer>>& GetVertexBuffers() const override final { return m_VertexBuffers; }

            virtual void SetIndexBuffer(const Ref<IndexBuffer>& buffer) override final;
            inline virtual const Ref<IndexBuffer>& GetIndexBuffer() const override final { return m_IndexBuffer; }

        private:
            u32 m_RendererID;

            u32 m_VertexBufferIndex;

            DynamicArray<Ref<VertexBuffer>> m_VertexBuffers;
            Ref<IndexBuffer> m_IndexBuffer;

        };  // VertexArray

    }   // Shrimp

}   // Ocean
