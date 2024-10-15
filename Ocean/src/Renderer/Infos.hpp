#pragma once

#include "Ocean/Core/Types/ValueTypes.hpp"

#include "Renderer/Shader.hpp"

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

		inline VkAttachmentDescription GetColorAttachmentDescription(
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

		inline VkAttachmentReference GetColorAttachmentReference(
		) {
			VkAttachmentReference colorReference{ };
			colorReference.attachment = 0;
			colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			return colorReference;
		}

		inline VkSubpassDescription GetSubpassAttachmentDescription(
		) {
			VkAttachmentReference colorRef = GetColorAttachmentReference();

			VkSubpassDescription subpass{ };
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &colorRef;

			return subpass;
		}

		inline VkSubpassDependency GetSubpassDependency() {
			VkSubpassDependency info{ };

			info.srcSubpass = VK_SUBPASS_EXTERNAL;
			info.dstSubpass = 0;

			info.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			info.srcAccessMask = 0;

			info.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			info.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			return info;
		}

		inline VkPipelineShaderStageCreateInfo GetVertextShaderStageInfo(
			Shader* vertexShader,
			VkDevice device,
			cstring name
		) {
			VkPipelineShaderStageCreateInfo info{ };
			info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			info.stage = VK_SHADER_STAGE_VERTEX_BIT;

			info.module = vertexShader->GetShaderModule(device);
			info.pName = name;

			return info;
		}

		inline VkPipelineShaderStageCreateInfo GetFragmentShaderStageInfo(
			Shader* fragmentShader,
			VkDevice device,
			cstring name
		) {
			VkPipelineShaderStageCreateInfo info{ };
			info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;

			info.module = fragmentShader->GetShaderModule(device);
			info.pName = name;

			return info;
		}



		/*VkClearValue GetClearColor(f32 r, f32 g, f32 b, f32 a) {
			return { { { r, g, b, a } } };
		}*/

	}	// Vulkan

}	// Ocean
