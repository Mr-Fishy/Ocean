#include "ocpch.hpp"

#include "Vk/VulkanFramebuffer.hpp"

#include "Vk/VulkanData.hpp"

namespace Ocean {

	void VulkanFramebuffer::Init() {
		CreateFramebuffers();
	}

	void VulkanFramebuffer::Destroy() {
		for (auto framebuffer : v_Framebuffers)
			vkDestroyFramebuffer(vkData->Device.GetLogical(), framebuffer, nullptr);
	}

	void VulkanFramebuffer::CreateFramebuffers() {
		v_Framebuffers.resize(vkData->Swapchain.GetImageViews().size());

		for (size_t i = 0; i < vkData->Swapchain.GetImageViews().size(); i++) {
			std::array< VkImageView, 2> attachments = { vkData->Swapchain.GetImageViews()[i], vkData->DepthTexture.GetView() };

			VkFramebufferCreateInfo info{};

			info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;

			info.renderPass = vkData->Renderer.GetRenderPass();

			info.attachmentCount = static_cast<ui32>(attachments.size());
			info.pAttachments = attachments.data();

			info.width = vkData->Swapchain.GetExtent().width;
			info.height = vkData->Swapchain.GetExtent().height;

			info.layers = 1;

			if (vkCreateFramebuffer(vkData->Device.GetLogical(), &info, nullptr, &v_Framebuffers[i]) != VK_SUCCESS)
				throw std::runtime_error("Failed to create Vulkan framebuffer!");
		}
	}

}	// Ocean

