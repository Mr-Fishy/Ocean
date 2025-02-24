#include "vk_Framebuffer.hpp"

#include "Ocean/Renderer/Framebuffer.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {

        vkFramebuffer::vkFramebuffer(const FramebufferSpecification& spec) :
            Framebuffer(spec),
            m_Framebuffer(0),
            m_ColorAttachments(0),
            m_DepthAttachment()
        {
            Invalidate();
        }

        vkFramebuffer::~vkFramebuffer() {
            
        }

        void vkFramebuffer::Bind() {
            
        }

        void vkFramebuffer::Unbind() {
            
        }

        void vkFramebuffer::Invalidate() {
            // ============================== DELETE OLD FRAMEBUFFER ==============================
            //


            // ============================== CREATE NEW FRAMEBUFFER ==============================
            //
            
        }

        void vkFramebuffer::Resize(u32 width, u32 height) {
            
        }

        u32 vkFramebuffer::ReadPixel(u32 attachmentIndex, i32 x, i32 y) {
            
        }

        void vkFramebuffer::ClearAttachment(u32 attachmentIndex, i32 value) {

        }

    }   // Splash

}   // Ocean
