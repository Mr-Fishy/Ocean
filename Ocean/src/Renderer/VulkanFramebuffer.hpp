#pragma once

#include "Ocean/Core/Primitives/Array.hpp"

#include "Renderer/VulkanResources.hpp"

namespace Ocean {

	namespace Vulkan {

		/**
		 * @brief The configuration struct of the framebuffer.
		 */
		struct FramebufferConfig {
			// Allocator* allocator;

			VkDevice device;
			VkImageView imageView;
			VkExtent2D swapchainExtent;
			VkRenderPass renderPass;

			FramebufferConfig(VkDevice device, VkImageView view, VkExtent2D extent, VkRenderPass renderPass)
				: device(device), imageView(view), swapchainExtent(extent), renderPass(renderPass) { }
		};

		/**
		 * @brief A overhead class that defines a Vulkan Framebuffer.
		 */
		class Framebuffer {
		public:
			Framebuffer() = default;
			~Framebuffer() = default;

			/**
			 * @brief Initializes the framebuffer, this includes creating the Vulkan Framebuffer.
			 * @param config - The configuration to use for the framebuffer.
			 */
			void Init(FramebufferConfig* config);
			/**
			 * @brief Shuts down the framebuffer, also destroys the Vulkan Framebuffer.
			 */
			void Shutdown();

			/**
			 * @return The Vulkan Framebuffer.
			 */
			VkFramebuffer GetFrame() const { return m_Framebuffer; }

		private:
			VkDevice m_DeviceRef = VK_NULL_HANDLE;

			VkFramebuffer m_Framebuffer = VK_NULL_HANDLE;
			VkRenderPass m_RenderPass = VK_NULL_HANDLE;
			VkSampler m_Sampler = VK_NULL_HANDLE;

			u32 m_Width = 0, m_Height = 0;

		};

	}	// Vulkan

}	// Ocean
