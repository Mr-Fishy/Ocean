#include "gl_Texture.hpp"

#include "Ocean/Primitives/Assert.hpp"
#include "Ocean/Renderer/Texture.hpp"

namespace Ocean {

    namespace Splash {

        glTexture2D::glTexture2D(u32 width, u32 height) : Texture2D(width, height), m_RendererID(0), m_InternalFormat(), m_DataFormat() {
            this->m_Width = width;
            this->m_Height = height;

            this->m_InternalFormat = GL_RGBA8;
            this->m_DataFormat = GL_RGBA;

            glCreateTextures(GL_TEXTURE_2D, 1, &this->m_RendererID);
            glTextureStorage2D(this->m_RendererID, 1, this->m_InternalFormat, this->m_Width, this->m_Height);

            glTextureParameteri(this->m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(this->m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        glTexture2D::~glTexture2D() {
            glDeleteTextures(1, &this->m_RendererID);
        }

        void glTexture2D::Bind(u32 slot) const {
            glBindTextureUnit(slot, this->m_RendererID);
        }

        void glTexture2D::SetData(void* data, u32 size) {
            u32 bpp = this->m_DataFormat == GL_RGBA ? 4 : 3;
            OASSERTM(size == this->m_Width * this->m_Height * bpp, "Data must be entire texture!");

            glTextureSubImage2D(this->m_RendererID, 0, 0, 0, this->m_Width, this->m_Height, this->m_DataFormat, GL_UNSIGNED_BYTE, data);
        }

        void glTexture2D::SetFormat(TextureFormat format) {
            switch (format) {
                case R:
                case G:
                case B:
                    break;

                case RGB:
                    this->m_InternalFormat = GL_RGB8;
                    this->m_DataFormat = GL_RGB;
                    break;

                case RGBA:
                    this->m_InternalFormat = GL_RGBA8;
                    this->m_DataFormat = GL_RGBA;
                    break;

                case A:
                    break;
            }
        }

    }   // Splash

}   // Ocean
