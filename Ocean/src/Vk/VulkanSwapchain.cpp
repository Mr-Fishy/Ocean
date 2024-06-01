#include "ocpch.hpp"

#include "Vk/VulkanSwapchain.hpp"

#include "Vk/VulkanData.hpp"

namespace Ocean {

	void VulkanSwapchain::Init() {
		CreateSwapchain();

		CreateImageViews();
	}

	void VulkanSwapchain::Destroy() {
		CleanSwapchain();
	}

	void VulkanSwapchain::CleanSwapchain() {
		vkData->DepthTexture.Destory();

		vkData->Framebuffer.Destroy();

		for (auto imageView : v_SwapchainImageViews)
			vkDestroyImageView(vkData->Device.GetLogical(), imageView, nullptr);

		vkDestroySwapchainKHR(vkData->Device.GetLogical(), m_Swapchain, nullptr);
	}

	void VulkanSwapchain::RecreateSwapchain() {
		i32 width = 0, height = 0;
		glfwGetFramebufferSize(vkData->Window, &width, &height);

		while (width == 0 || height == 0) {
			glfwGetFramebufferSize(vkData->Window, &width, &height);
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(vkData->Device.GetLogical());

		CleanSwapchain();

		CreateSwapchain();
		CreateImageViews();

		vkData->DepthTexture.Init();
		
		vkData->Framebuffer.Init();
	}

	/* HELPER FUNCTIONS */

	static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		for (const auto& format : availableFormats) {
			if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				return format;
		}

		return availableFormats[0];
	}

	static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
		for (const auto& presentMode : availablePresentModes) {
			if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				return presentMode;
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	static VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
		if (capabilities.currentExtent.width != UINT32_MAX) {
			return capabilities.currentExtent;
		}

		i32 width, height;
		glfwGetFramebufferSize(vkData->Window, &width, &height);

		VkExtent2D actualExtent = {
			static_cast<ui32>(width),
			static_cast<ui32>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}

	/* ---------------- */

	void VulkanSwapchain::CreateSwapchain() {
		SwapchainSupportDetails swapChainSupport = SwapchainSupportDetails::QuerySwapchainSupport(vkData->Device.GetPhysical(), vkData->Surface);

		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.Formats);
		VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.PresentModes);
		VkExtent2D extent = ChooseSwapExtent(swapChainSupport.Capabilities);

		ui32 imageCount = swapChainSupport.Capabilities.minImageCount + 1;
		if (swapChainSupport.Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.Capabilities.maxImageCount)
			imageCount = swapChainSupport.Capabilities.maxImageCount;

		VkSwapchainCreateInfoKHR info{};

		info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		info.surface = vkData->Surface;

		info.minImageCount = imageCount;
		info.imageFormat = surfaceFormat.format;
		info.imageColorSpace = surfaceFormat.colorSpace;
		info.imageExtent = extent;
		info.imageArrayLayers = 1; // Always 1 unless rendering for stereoscopic video
		info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = QueueFamilyIndices::FindQueueFamilies(vkData->Device.GetPhysical(), vkData->Surface);
		ui32 queueFamilyIndices[] = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

		if (indices.GraphicsFamily != indices.PresentFamily) {
			info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			info.queueFamilyIndexCount = 2;
			info.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			info.queueFamilyIndexCount = 0;			// Optional
			info.pQueueFamilyIndices = nullptr;		// Optional
		}

		info.preTransform = swapChainSupport.Capabilities.currentTransform;
		info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		info.presentMode = presentMode;
		info.clipped = VK_TRUE;

		info.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(vkData->Device.GetLogical(), &info, nullptr, &m_Swapchain) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan swap chain!");

		vkGetSwapchainImagesKHR(vkData->Device.GetLogical(), m_Swapchain, &imageCount, nullptr);
		v_SwapchainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(vkData->Device.GetLogical(), m_Swapchain, &imageCount, v_SwapchainImages.data());

		m_SwapchainImageFormat = surfaceFormat.format;
		m_SwapchainExtent = extent;
	}

	void VulkanSwapchain::CreateImageViews() {
		v_SwapchainImageViews.resize(v_SwapchainImages.size());

		for (ui64 i = 0; i < v_SwapchainImages.size(); i++) {
			v_SwapchainImageViews[i] = VkUtils::CreateImageView(v_SwapchainImages[i], m_SwapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
		}
	}

}	// Ocean
