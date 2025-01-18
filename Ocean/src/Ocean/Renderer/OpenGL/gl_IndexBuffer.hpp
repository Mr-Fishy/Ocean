#pragma once

#include "Ocean/Renderer/IndexBuffer.hpp"

namespace Ocean {

    namespace Shrimp {
    
        class glIndexBuffer : public IndexBuffer {
        public:
            glIndexBuffer(u32* indices, u32 count);
            virtual ~glIndexBuffer();

            virtual void Bind() const override final;
            virtual void Unbind() const override final;

            inline virtual u32 GetCount() const override final { return this->m_Count; }

        private:
            u32 m_RendererID;
            u32 m_Count;

        };

    }   // Shrimp

}   // Ocean
