#include "vk_RenderPass.hpp"

#include "Ocean/Types/Integers.hpp"

#include "Ocean/Primitives/Array.hpp"

#include "Ocean/Renderer/Vulkan/vk_Vulkan.hpp"
#include "Ocean/Renderer/Vulkan/vk_Instance.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {

        vkRenderPass::vkAttachment::vkAttachment(AttachmentType type) :
            m_Description(),
            m_Reference()
        {
            // ============================== ATTACHMENT DESCRIPTION ==============================
            //
            this->m_Description.format = vkInstance::Get().Swapchain()->Format();
            this->m_Description.samples = VK_SAMPLE_COUNT_1_BIT;

            this->m_Description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            this->m_Description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

            this->m_Description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            this->m_Description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

            this->m_Description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            this->m_Description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

            // ============================== ATTACHMENT REFERENCE ==============================
            //
            this->m_Reference.attachment = 0;
            this->m_Reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }

        vkRenderPass::vkAttachment::~vkAttachment()
        {
            
        }

        vkRenderPass::vkRenderPass() :
            m_Pass(VK_NULL_HANDLE),
            m_Attachments(0)
        {

        }

        vkRenderPass::~vkRenderPass() {
            
        }

        void vkRenderPass::Invalidate() {
            // ============================== ATTACHMENTS ==============================
            //
            DynamicArray<VkAttachmentDescription> attachments(this->m_Attachments.size());
            DynamicArray<VkAttachmentReference> attachmentReferences(this->m_Attachments.size());

            for (const vkAttachment& attachment : this->m_Attachments) {
                attachments.emplace_back(attachment.Description());
                attachmentReferences.emplace_back(attachment.Reference());
            }

            // ============================== SUB-PASSES ==============================
            //
            VkSubpassDescription subpass {
                0,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                0,
                0,
                static_cast<u32>(attachmentReferences.size()),
                attachmentReferences.data(),
                nullptr,
                nullptr,
                0,
                nullptr
            };

            // ============================== RENDER-PASS ==============================
            //
            VkRenderPassCreateInfo renderPassInfo {
                VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
                nullptr,
                0,
                static_cast<u32>(attachments.size()),
                attachments.data(),
                1,
                &subpass,
                0,
                nullptr,
            };

            vkCheck(
                vkCreateRenderPass(vkInstance::Get().Device()->Logical(), &renderPassInfo, nullptr, &this->m_Pass)
            );
        }

    } // namespace Splash

} // namespace Ocean
