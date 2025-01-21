#include "gl_Framebuffer.hpp"

#include "Ocean/Types/Integers.hpp"

#include "Ocean/Primitives/Assert.hpp"
#include "Ocean/Primitives/Log.hpp"
#include "Ocean/Primitives/Macros.hpp"

#include "Ocean/Renderer/Framebuffer.hpp"

// libs
#include <glad/gl.h>

namespace Ocean {

    namespace Shrimp {

        static GLenum TextureTarget(b8 multisampled) {
            return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        }

        static void CreateTextures(b8 multisampled, u32* outID, u32 count) {
            glCreateTextures(TextureTarget(multisampled), count, outID);
        }

        static void BindTexture(b8 multisampled, u32 id) {
            glBindTexture(TextureTarget(multisampled), id);
        }

        static void AttachColorTexture(u32 id, i32 samples, GLenum internalFormat, GLenum format, u32 width, u32 height, i32 index) {
            b8 multisampled = samples > 1;

            if (multisampled) {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
            }
            else {
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
        }

        static void AttachDepthTexture(u32 id, i32 samples, GLenum format, GLenum attachmentType, u32 width, u32 height) {
            b8 multisampled = samples > 1;

            if (multisampled) {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
            }
            else {
                glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
        }

        static b8 IsDepthFormat(FramebufferFormat format) {
            switch (format) {
                case FramebufferFormat::None:                
                case FramebufferFormat::RGBA8:                
                case FramebufferFormat::Red_Int:
                    break;
                
                case FramebufferFormat::Depth24_Stencil8:
                    return true;
            }

            return false;
        }

        static GLenum OceanFramebufferTextureFormatToGL(FramebufferFormat format) {
            switch (format) {
                case FramebufferFormat::None:
                    break;

                case FramebufferFormat::RGBA8:
                    return GL_RGBA8;

                case FramebufferFormat::Red_Int:
                    return GL_RED_INTEGER;

                case FramebufferFormat::Depth24_Stencil8:
                    break;
            }

            OASSERTM(false, "Reached \"unreachable\" return due to unkown or invalid framebuffer texture format!");
            return 0;
        }



        #define MAX_FRAMEBUFFER_SIZE 8192

        glFramebuffer::glFramebuffer(const FramebufferSpecification& spec) : m_RendererID(0), m_Specification(spec), m_ColorAttachmentSpecs(), m_DepthAttachmentSpec(), m_ColorAttachments(), m_DepthAttachment(0) {
            for (auto& textureSpec : this->m_Specification.attachments.attachments) {
                if (!IsDepthFormat(textureSpec.textureFormat))
                    this->m_ColorAttachmentSpecs.EmplaceBack(textureSpec);
                else
                    this->m_DepthAttachmentSpec = textureSpec;
            }

            Invalidate();
        }

        glFramebuffer::~glFramebuffer() {
            glDeleteFramebuffers(1, &this->m_RendererID);

            glDeleteTextures(static_cast<GLsizei>(this->m_ColorAttachments.Size()), this->m_ColorAttachments.Data());
            glDeleteTextures(1, &this->m_DepthAttachment);
        }

        void glFramebuffer::Bind() {
            glBindFramebuffer(GL_FRAMEBUFFER, this->m_RendererID);

            glViewport(0, 0, this->m_Specification.width, this->m_Specification.height);
        }

        void glFramebuffer::Unbind() {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void glFramebuffer::Invalidate() {
            if (this->m_RendererID) {
                glDeleteFramebuffers(1, &this->m_RendererID);

                glDeleteTextures(static_cast<GLsizei>(this->m_ColorAttachments.Size()), this->m_ColorAttachments.Data());
                glDeleteTextures(1, &this->m_DepthAttachment);

                this->m_ColorAttachments.Clear();
                this->m_DepthAttachment = 0;
            }

            glCreateFramebuffers(1, &this->m_RendererID);
            glBindFramebuffer(GL_FRAMEBUFFER, this->m_RendererID);

            b8 multisample = this->m_Specification.samples > 1;

            if (!this->m_ColorAttachmentSpecs.Empty()) {
                this->m_ColorAttachments.Resize(this->m_ColorAttachmentSpecs.Size());
                
                CreateTextures(multisample, this->m_ColorAttachments.Data(), this->m_ColorAttachments.Size());

                for (u32 i = 0; i < this->m_ColorAttachments.Size(); i++) {
                    BindTexture(multisample, this->m_ColorAttachments[i]);

                    switch (this->m_ColorAttachmentSpecs[i].textureFormat) {
                        case FramebufferFormat::None:
                        case FramebufferFormat::Depth24_Stencil8:
                          break;

                        case FramebufferFormat::RGBA8:
                            AttachColorTexture(this->m_ColorAttachments[i], this->m_Specification.samples, GL_RGBA8, GL_RGBA, this->m_Specification.width, this->m_Specification.height, i);
                            break;

                        case FramebufferFormat::Red_Int:
                            AttachColorTexture(this->m_ColorAttachments[i], this->m_Specification.samples, GL_R32I, GL_RED_INTEGER, this->m_Specification.width, this->m_Specification.height, i);
                            break;
                    }
                }
            }

            if (this->m_DepthAttachmentSpec.textureFormat != FramebufferFormat::None) {
                CreateTextures(multisample, &this->m_DepthAttachment, 1);
                BindTexture(multisample, this->m_DepthAttachment);

                switch (this->m_DepthAttachmentSpec.textureFormat) {
                    case None:
                    case RGBA8:
                    case Red_Int:
                        break;

                    case Depth24_Stencil8:
                        AttachDepthTexture(this->m_DepthAttachment, this->m_Specification.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, this->m_Specification.width, this->m_Specification.height);
                        break;
                }
            }

            if (this->m_ColorAttachments.Size() > 1) {
                OASSERTM(this->m_ColorAttachments.Size() <= 4, "Attempted to use more than optimal framebuffer attachment count!");

                GLenum buffers[4] = {
                    GL_COLOR_ATTACHMENT0,
                    GL_COLOR_ATTACHMENT1,
                    GL_COLOR_ATTACHMENT2,
                    GL_COLOR_ATTACHMENT3,
                };
                glDrawBuffers(static_cast<GLsizei>(this->m_ColorAttachments.Size()), buffers);
            }
            else if (this->m_ColorAttachments.Empty()) {
                glDrawBuffer(GL_NONE); // Only Depth-Pass
            }

            OASSERTM(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void glFramebuffer::Resize(u32 width, u32 height) {
            if (width == 0 || height == 0 || width > MAX_FRAMEBUFFER_SIZE || height > MAX_FRAMEBUFFER_SIZE) {
                oprint(CONSOLE_TEXT_YELLOW("Attempted to resize framebuffer to %i x %i"), width, height);

                return;
            }

            this->m_Specification.width = width;
            this->m_Specification.height = height;

            Invalidate();
        }
        
        u32 glFramebuffer::ReadPixel(u32 attachmentIndex, i32 x, i32 y) {
            OASSERT_LENGTH(attachmentIndex, this->m_ColorAttachments.Size());

            glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);

            i32 pixelData = -1;
            glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

            return pixelData;
        }

        void glFramebuffer::ClearAttachment(u32 attachmentIndex, i32 value) {
            OASSERT_LENGTH(attachmentIndex, this->m_ColorAttachments.Size());
            
            auto& spec = this->m_ColorAttachmentSpecs[attachmentIndex];
            glClearTexImage(
                this->m_ColorAttachments[attachmentIndex],
                0,
                OceanFramebufferTextureFormatToGL(spec.textureFormat),
                GL_INT,
                &value
            );
        }

    }   // Shrimp

}   // Ocean
