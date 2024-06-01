#pragma once

#include "Vk/VulkanData.hpp"

// libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Ocean {

	/**
	 * @brief The Vulkan rendering context and backend manager.
	 * @details Order of operations
	 * (1) VulkanInstance
	 *     - Create Debugger
	 *     - Create Surface
	 * (2) VulkanDevice
	 *     - Pick Physical Device
	 *     - Create Logical Device
	 * (3) VulkanSwapchain
	 *     - Create Swapchain
	 *     - Create Image Views
	 * (4) VulkanRenderer
	 *     - Create Render Pass
	 *     - Create Descriptor Layout (Through VulkanDescriptor)
	 *     - Create Graphics Pipeline
	 * (5) VulkanFramebuffer
	 *     - Create Framebuffers
	 * (6) VulkanBuffer
	 *     - Create Command Pool
	 *     - Create Command Buffer
	 * (7) VulkanTexture(s)
	 *     - Create Texture Image
	 *     - Create Texture Image View
	 * (8) VulkanSampler(s)
	 *     - Create Texture Sampler
	 * (9) VulkanVertexBuffer
	 *     - Create Vertex Buffer
	 * (10)VulkanIndexBuffer
	 *     - Create Index Buffer
	 * (11)VulkanDescriptor
	 *     - Create Descriptor Pool
	 *     - Create Descriptor Sets
	 * (12)VulkanSync
	 *     - Create Sync Objects
	 */
	class VulkanContext {
	public:
		VulkanContext(GLFWwindow* window);
		~VulkanContext();

		void DrawFrame();

		void FramebufferResized() { m_FramebufferResized = true; }

		VkDevice GetDevice() const;

	private:
		b8 m_FramebufferResized = false;
	};

}	// Ocean
