#include "Framebuffer.hpp"

#include "Renderer/Components/VkTypes.hpp"

namespace Ocean {

	namespace Vulkan {

		void Framebuffer::Init(FramebufferConfig* config) {
			m_DeviceRef = config->device;
			m_RenderPass = config->renderPass;

			m_Width = config->swapchainExtent.width;
			m_Height = config->swapchainExtent.height;

			Framebuffers::CreateInfo createInfo(
				m_RenderPass,
				1,
				1,
				&config->imageView,
				m_Width,
				m_Height
			);

			CHECK_RESULT(
				vkCreateFramebuffer(m_DeviceRef, &createInfo, nullptr, &m_Framebuffer),
				"Failed to create framebuffer!"
			);
		}

		void Framebuffer::Shutdown() {
			vkDestroyFramebuffer(m_DeviceRef, m_Framebuffer, nullptr);
		}

	}	// Vulkan

}	// Ocean
