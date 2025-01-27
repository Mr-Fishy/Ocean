#pragma once

/**
 * @file vk_Framebuffer.hpp
 * @author Evan F.
 * @brief 
 * @date 2025-01-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Types/Integers.hpp"

#include "Ocean/Primitives/Assert.hpp"
#include "Ocean/Primitives/Array.hpp"

#include "Ocean/Renderer/Framebuffer.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {

        class vkFramebuffer : public Framebuffer {
        public:
            vkFramebuffer(const FramebufferSpecification& spec);
            virtual ~vkFramebuffer();

            virtual void Bind() override final;
            virtual void Unbind() override final;

            void Invalidate();

            virtual void Resize(u32 width, u32 height) override final;
            
            virtual u32 ReadPixel(u32 attachmentIndex, i32 x, i32 y) override final;

            inline virtual u32 GetColorAttachmentID(u32 index = 0) const override final { OASSERT_LENGTH(index, this->m_ColorAttachments.size()); return this->m_ColorAttachments[index]; }
            virtual void ClearAttachment(u32 attachmentIndex, i32 value) override final;

            inline virtual const FramebufferSpecification& GetSpecification() const override final { return this->m_Specification; }

        private:
            VkFramebuffer m_Framebuffer;

            VkRenderPass m_RenderPass;
            
            VkImage m_ColorImage;
            VkImageView m_ColorImageView;

            FramebufferSpecification m_Specification;

            DynamicArray<FramebufferTextureSpec> m_ColorAttachmentSpecs;
            FramebufferTextureSpec m_DepthAttachmentSpec;

            DynamicArray<u32> m_ColorAttachments;
            u32 m_DepthAttachment;

        };  // Framebuffer

    }   // Splash

}   // Ocean