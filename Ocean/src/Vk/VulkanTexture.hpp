#pragma once

// libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Ocean {

	class VulkanTexture {
	public:
		VulkanTexture();
		~VulkanTexture();

		VkImage GetImage() const { return m_Image; }
		VkImageView GetView() const { return m_ImageView; }
		VkDeviceMemory GetMemory() const { return m_ImageMemory; }

	private:
		void CreateTextureImage();

		void CreateTextureImageView();

		/* --- */

		VkImage m_Image;
		VkImageView m_ImageView;

		VkDeviceMemory m_ImageMemory;
	};

	class VulkanDepthTexture {
	public:
		VulkanDepthTexture() : m_Image(VK_NULL_HANDLE), m_ImageView(VK_NULL_HANDLE), m_ImageMemory(VK_NULL_HANDLE) { }
		~VulkanDepthTexture() = default;

		void Init();
		void Destory() const;

		VkImage GetImage() const { return m_Image; }
		VkImageView GetView() const { return m_ImageView; }
		VkDeviceMemory GetMemory() const { return m_ImageMemory; }

		VkFormat FindDepthFormat();

	private:
		void CreateDepthResources();

		/* --- */

		VkImage m_Image;
		VkImageView m_ImageView;

		VkDeviceMemory m_ImageMemory;
	};

}	// Ocean
