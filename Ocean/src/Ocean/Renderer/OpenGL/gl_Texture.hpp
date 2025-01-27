#pragma once

#include "Ocean/Renderer/Texture.hpp"

// libs
#include <glad/gl.h>

namespace Ocean {

    namespace Splash {

        class glTexture2D : public Texture2D {
        public:
            glTexture2D(u32 width, u32 height);
            virtual ~glTexture2D();

            virtual void Bind(u32 slot) const override final;

            virtual void SetData(void* data, u32 size) override final;
            virtual void SetFormat(TextureFormat format) override final;

            inline virtual u32 GetWidth() const { return this->m_Width; }
            inline virtual u32 GetHeight() const { return this->m_Height; }

            inline virtual u32 GetRendererID() const override final { return this->m_RendererID; }

            inline virtual bool operator == (const Texture& other) const override final { return this->m_RendererID == static_cast<const glTexture2D&>(other).m_RendererID; }

        private:
            u32 m_RendererID;

            GLenum m_InternalFormat, m_DataFormat;

        };  // Texture2D

    }   // Splash

}   // Ocean
