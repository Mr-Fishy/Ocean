#pragma once

/**
 * @file gl_Framebuffer.hpp
 * @author Evan F.
 * @brief The implementation of an OpenGL Framebuffer.
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Types/Integers.hpp"

#include "Ocean/Primitives/Assert.hpp"
#include "Ocean/Primitives/DynamicArray.hpp"

#include "Ocean/Renderer/Framebuffer.hpp"

namespace Ocean {

    namespace Splash {

        /**
         * @brief An OpenGL implemented Framebuffer.
         */
        class glFramebuffer : public Framebuffer {
        public:
            /** @copydoc Framebuffer::Framebuffer() */
            glFramebuffer(const FramebufferSpecification& spec);
            virtual ~glFramebuffer();

            /** @copydoc Framebuffer::Bind() */
            virtual void Bind() override final;
            /** @copydoc Framebuffer::Unbind() */
            virtual void Unbind() override final;

            /** @copydoc Framebuffer::Invalidate() */
            virtual void Invalidate() override final;

            /** @copydoc Framebuffer::Resize() */
            virtual void Resize(u32 width, u32 height) override final;
            
            /** @copydoc Framebuffer::ReadPixel() */
            virtual u32 ReadPixel(u32 attachmentIndex, i32 x, i32 y) override final;

            /** @copydoc Framebuffer::GetColorAttachmentID() */
            inline virtual u32 GetColorAttachmentID(u32 index = 0) const override final {
                OASSERT_LENGTH(index, this->m_ColorAttachments.Size());

                return this->m_ColorAttachments[index];
            }
            /** @copydoc Framebuffer::ClearAttachment() */
            virtual void ClearAttachment(u32 attachmentIndex, i32 value) override final;

        private:
            u32 m_RendererID; /** @brief The OpenGL ID of the framebuffer object. */

            DynamicArray<u32> m_ColorAttachments; /** @brief The OpenGL color attachments of the framebuffer object. */
            u32 m_DepthAttachment; /** @brief The OpenGL depth attachment of the framebuffer object. */

        };  // Framebuffer

    }   // Splash

}   // Ocean
