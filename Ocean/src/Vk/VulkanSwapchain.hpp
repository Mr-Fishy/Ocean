#pragma once

#include "Core/Types.hpp"

// libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Ocean {

	class VulkanSwapchain {
	public:
		VulkanSwapchain() : m_Swapchain(VK_NULL_HANDLE), m_SwapchainImageFormat{}, m_SwapchainExtent{} { }
		~VulkanSwapchain() = default;

		void Init();
		void Destroy();

		void CleanSwapchain();
		void RecreateSwapchain();

		VkSwapchainKHR GetSwapchain() const { return m_Swapchain; }

		VkFormat GetFormat() const { return m_SwapchainImageFormat; }
		VkExtent2D GetExtent() const { return m_SwapchainExtent; }

		std::vector<VkImageView> GetImageViews() const { return v_SwapchainImageViews; }

	private:
		void CreateSwapchain();

		void CreateImageViews();

		/* --- */

		VkSwapchainKHR m_Swapchain;

		VkFormat m_SwapchainImageFormat;
		VkExtent2D m_SwapchainExtent;

		std::vector<VkImage> v_SwapchainImages;
		std::vector<VkImageView> v_SwapchainImageViews;
	};

}	// Ocean
