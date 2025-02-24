#pragma once

/**
 * @file vk_RenderPass.hpp
 * @author Evan F.
 * @brief 
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Primitives/Array.hpp"
#include "Ocean/Primitives/Macros.hpp"

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

                OC_INLINE const VkAttachmentDescription& Description() const { return this->m_Description; }
                OC_INLINE const VkAttachmentReference& Reference() const { return this->m_Reference; }

            private:
                VkAttachmentDescription m_Description;

                VkAttachmentReference m_Reference;

            };  // vkAttachment

        public:
            vkRenderPass();
            ~vkRenderPass();

            OC_INLINE void AddAttachment(AttachmentType type) { this->m_Attachments.emplace_back(type); }
            OC_INLINE vkAttachment& GetAttachment(u32 index) { return this->m_Attachments[index]; }
            OC_INLINE void RemoveAttachment(u32 index) { this->m_Attachments.erase(this->m_Attachments.begin() + index); }

            OC_INLINE VkRenderPass RenderPass() const { return this->m_Pass; }

            void Invalidate();

        private:
            VkRenderPass m_Pass;

            DynamicArray<vkAttachment> m_Attachments;
    
        };  // vkRenderPass

    }   // Splash

}   // Ocean
