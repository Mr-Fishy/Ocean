#pragma once

/**
 * @file vk_RenderPass.hpp
 * @author Evan F.
 * @brief 
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Primitives/DynamicArray.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {

        typedef enum AttachmentType {
            COLOR,
            DEPTH,

        } AttachmentType;
    
        class vkRenderPass {
        private:
            class vkAttachment {
            public:
                vkAttachment() = default;
                vkAttachment(AttachmentType type);
                ~vkAttachment();

                inline const VkAttachmentDescription& Description() const { return this->m_Description; }
                inline const VkAttachmentReference& Reference() const { return this->m_Reference; }

                b8 operator == (const vkAttachment &other) const {
                    return this->m_Description.format == other.m_Description.format
                        && this->m_Description.initialLayout == other.m_Description.initialLayout
                        && this->m_Description.finalLayout == other.m_Description.finalLayout
                        && this->m_Reference.attachment == other.m_Reference.attachment
                        && this->m_Reference.layout == other.m_Reference.layout;
                }
                b8 operator != (const vkAttachment &other) const {
                    return !(*this == other);
                }

            private:
                VkAttachmentDescription m_Description;

                VkAttachmentReference m_Reference;

            };  // vkAttachment

        public:
            vkRenderPass();
            ~vkRenderPass();

            inline void AddAttachment(AttachmentType type) { this->m_Attachments.EmplaceBack(type); }
            inline vkAttachment& GetAttachment(u32 index) { return this->m_Attachments[index]; }
            inline void RemoveAttachment(u32 index) { this->m_Attachments.Erase(index); }

            inline VkRenderPass RenderPass() const { return this->m_Pass; }

            void Invalidate();

            b8 operator == (const vkRenderPass& other) const {
                return this->m_Pass == other.m_Pass
                    && this->m_Attachments == other.m_Attachments;
            }
            b8 operator != (const vkRenderPass& other) const {
                return !(*this == other);
            }

        private:
            VkRenderPass m_Pass;

            DynamicArray<vkAttachment> m_Attachments;
    
        };  // vkRenderPass

    }   // Splash

}   // Ocean
