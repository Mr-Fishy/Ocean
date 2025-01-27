#pragma once

#include "Ocean/Types/FloatingPoints.hpp"

#include "Ocean/Renderer/VertexBuffer.hpp"

namespace Ocean {

    namespace Splash {

        class glVertexBuffer : public VertexBuffer {
        public:
            glVertexBuffer(u32 size);
            glVertexBuffer(f32* vertices, u32 size);
            virtual ~glVertexBuffer();

            virtual void Bind() const override final;
            virtual void Unbind() const override final;

            virtual void SetData(const void* data, u32 size) override final;

            inline virtual const BufferLayout& GetLayout() const override final { return this->m_Layout; }
            inline virtual void SetLayout(const BufferLayout& layout) override final { this->m_Layout = layout; }

        private:
            u32 m_RendererID;

            BufferLayout m_Layout;

        };  // glVertexBuffer

    }   // Splash

}   // Ocean
