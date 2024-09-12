#pragma once

#include "Ocean/Core/Types/ValueTypes.hpp"

#pragma warning(push, 0)
#pragma warning(disable : 26495) // Uninitialized Warning
#include <vulkan/vulkan.hpp>
// #include "vk_mem_alloc.h"
#pragma warning(pop)

namespace Ocean {

	namespace Vulkan {

		inline VkImageViewCreateInfo ColorAttachmentCreateInfo(
			VkFormat colorFormat,
			VkImage image
		) {
			VkImageViewCreateInfo info{ };
			info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

			info.pNext = nullptr;

			info.format = colorFormat;
			info.components = {
				VK_COMPONENT_SWIZZLE_R,
				VK_COMPONENT_SWIZZLE_G,
				VK_COMPONENT_SWIZZLE_B,
				VK_COMPONENT_SWIZZLE_A
			};

			info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			info.subresourceRange.baseMipLevel = 0;
			info.subresourceRange.levelCount = 1;
			info.subresourceRange.baseArrayLayer = 0;
			info.subresourceRange.layerCount = 1;

			info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			info.flags = 0;

			info.image = image;

			return info;
		}

		inline VkAttachmentDescription ColorAttachmentDescription(
			VkFormat colorFormat
		) {
			VkAttachmentDescription colorAttachment{ };
			colorAttachment.format = colorFormat;
			colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

			colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

			colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

			colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			return colorAttachment;
		}

		inline VkSubpassDescription SubpassAttachmentDescription(
		) {
			VkAttachmentReference colorRef{ };
			colorRef.attachment = 0;
			colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			VkSubpassDescription subpass{ };
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &colorRef;

			return subpass;
		}

		/**
		 * @brief 
		 * @param commandPool 
		 * @param level 
		 * @param bufferCount 
		 * @return 
		 */
		inline VkCommandBufferAllocateInfo CommandBufferAllocateInfo(
			VkCommandPool commandPool,
			VkCommandBufferLevel level,
			u32 bufferCount
		) {
			VkCommandBufferAllocateInfo info{ };
			info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

			info.commandPool = commandPool;

			info.level = level;
			info.commandBufferCount = bufferCount;

			return info;
		}



	}	// Vulkan

}	// Ocean
