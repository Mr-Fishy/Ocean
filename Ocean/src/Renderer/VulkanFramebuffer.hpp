#pragma once

#include "Ocean/Core/Primitives/Array.hpp"

#include "Renderer/VulkanResources.hpp"

namespace Ocean {

	namespace Vulkan {

		struct FramebufferAttachment {
			VkImage Image;
			VkDeviceMemory Memory;
			VkImageView View;
			VkFormat Format;
			VkImageSubresourceRange SubresourceRange;
			VkAttachmentDescription Description;

			b8 HasDepth();
			b8 HasStencil();
			b8 IsDepthOrStencil();
		};



		struct FramebufferConfig {
			Allocator* MemAllocator;

			VkDevice Device;
			VkImageView ImageView;
			VkExtent2D SwapchainExtent;
			VkRenderPass RenderPass;
		};

		class Framebuffer {
		public:
			Framebuffer() = default;
			~Framebuffer() = default;

			void Init(FramebufferConfig* config);
			void Shutdown();

			VkFramebuffer GetFrame() const { return m_Framebuffer; }

		private:
			VkDevice m_DeviceRef;

			VkFramebuffer m_Framebuffer;
			VkRenderPass m_RenderPass;
			VkSampler m_Sampler;

			u32 m_Width, m_Height;

			DynamicArray<FramebufferAttachment> m_Attachments;

		};

	}	// Vulkan

}	// Ocean
