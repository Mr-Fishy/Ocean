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

#include "Ocean/Primitives/DynamicArray.hpp"

#include "Ocean/Renderer/Framebuffer.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {

        class vkFramebuffer : public Framebuffer {
        private:
            struct FramebufferAttachment {
                VkImage image;
                VkImageView view;
                
                VkFormat format;

                VkDeviceMemory mem;

                b8 operator == (const FramebufferAttachment &other) const {
                    return this->image == other.image
                        && this->view == other.view
                        && this->format == other.format
                        && this->mem == other.mem;
                }
                b8 operator != (const FramebufferAttachment &other) const {
                    return !(*this == other);
                }

            };  // FramebufferAttachment

        public:
            vkFramebuffer(const FramebufferSpecification& spec);
            virtual ~vkFramebuffer();

            virtual void Bind() override final;
            virtual void Unbind() override final;

            virtual void Invalidate() override final;

            virtual void Resize(u32 width, u32 height) override final;
            
            virtual u32 ReadPixel(u32 attachmentIndex, i32 x, i32 y) override final;

            inline virtual u32 GetColorAttachmentID(u32 index = 0) const override final { return index; }
            virtual void ClearAttachment(u32 attachmentIndex, i32 value) override final;

        private:
            DynamicArray<VkFramebuffer> m_Framebuffer; /** @brief The list of available framebuffers. @details This should be the same size (count) as the vkSwapchain images. */
            VkSampler m_Sampler;

            VkRenderPass m_RenderPass;

            u32 m_Width;
            u32 m_Height;

            DynamicArray<FramebufferAttachment> m_ColorAttachments;
            FramebufferAttachment m_DepthAttachment;

        };  // vkFramebuffer

    }   // Splash

}   // Ocean