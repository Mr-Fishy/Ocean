#include "VulkanFramebuffer.hpp"

namespace Ocean {

	namespace Vulkan {

		void Framebuffer::Init(FramebufferConfig* config) {
			m_DeviceRef = config->device;
			m_RenderPass = config->renderPass;

			m_Width = config->swapchainExtent.width;
			m_Height = config->swapchainExtent.height;

			VkFramebufferCreateInfo info{ };
			info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;

			info.renderPass = m_RenderPass;

			info.attachmentCount = 1;
			info.pAttachments = &config->imageView;

			info.width = m_Width;
			info.height = m_Height;

			info.layers = 1;

			CHECK_RESULT(
				vkCreateFramebuffer(m_DeviceRef, &info, nullptr, &m_Framebuffer),
				"Failed to create framebuffer!"
			);
		}

		void Framebuffer::Shutdown() {
			vkDestroyFramebuffer(m_DeviceRef, m_Framebuffer, nullptr);
		}

	}	// Vulkan

}	// Ocean
