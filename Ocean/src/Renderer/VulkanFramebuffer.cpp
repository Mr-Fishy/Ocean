#include "VulkanFramebuffer.hpp"

namespace Ocean {

	namespace Vulkan {

		void Framebuffer::Init(FramebufferConfig* config) {
			m_DeviceRef = config->Device;
			m_RenderPass = config->RenderPass;

			m_Width = config->SwapchainExtent.width;
			m_Height = config->SwapchainExtent.height;

			VkFramebufferCreateInfo info{ };
			info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;

			info.renderPass = m_RenderPass;

			info.attachmentCount = 1;
			info.pAttachments = &config->ImageView;

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
