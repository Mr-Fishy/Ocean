#include "ocpch.hpp"

#include "Vk/VulkanTexture.hpp"

#include "Vk/VulkanData.hpp"

// libs
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Ocean {

	VulkanTexture::VulkanTexture() : m_Image(VK_NULL_HANDLE), m_ImageView(VK_NULL_HANDLE), m_ImageMemory(VK_NULL_HANDLE) {
		CreateTextureImage();

		CreateTextureImageView();
	}

	VulkanTexture::~VulkanTexture() {
		vkDestroyImageView(vkData->Device.GetLogical(), m_ImageView, nullptr);

		vkDestroyImage(vkData->Device.GetLogical(), m_Image, nullptr);
		vkFreeMemory(vkData->Device.GetLogical(), m_ImageMemory, nullptr);
	}

	/* HELPER FUNCTIONS */

	static b8 HasStencilComponent(VkFormat format) {
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}

	static VkFormat FindSupportedFormat(const std::vector<VkFormat>& condidates, VkImageTiling tiling, VkFormatFeatureFlags featureFlags) {
		for (VkFormat format : condidates) {
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(vkData->Device.GetPhysical(), format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & featureFlags) == featureFlags)
				return format;
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & featureFlags) == featureFlags)
				return format;
		}

		throw std::runtime_error("Failed to find supported Vulkan depth format!");
	}

	static void CreateImage(ui32 width, ui32 height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& memory) {
		VkImageCreateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		info.imageType = VK_IMAGE_TYPE_2D;

		info.extent.width = width;
		info.extent.height = height;
		info.extent.depth = 1;

		info.mipLevels = 1;
		info.arrayLayers = 1;

		info.format = format;
		info.tiling = tiling;

		info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		info.usage = usage;
		info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		info.samples = VK_SAMPLE_COUNT_1_BIT;

		info.flags = 0; // Optional

		if (vkCreateImage(vkData->Device.GetLogical(), &info, nullptr, &image) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan image!");

		VkMemoryRequirements memReq;
		vkGetImageMemoryRequirements(vkData->Device.GetLogical(), image, &memReq);

		VkMemoryAllocateInfo allocInfo{};

		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memReq.size;
		allocInfo.memoryTypeIndex = VkUtils::FindMemoryType(memReq.memoryTypeBits, properties, vkData->Device.GetPhysical());

		if (vkAllocateMemory(vkData->Device.GetLogical(), &allocInfo, nullptr, &memory) != VK_SUCCESS)
			throw std::runtime_error("Failed to allocate Vulkan image memory!");

		vkBindImageMemory(vkData->Device.GetLogical(), image, memory, 0);
	}

	static void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
		VkCommandBuffer commandBuffer = vkData->Buffer.BeginSingleCommand();

		VkImageMemoryBarrier barrier{};

		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;

		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		barrier.image = image;

		if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

			if (HasStencilComponent(format))
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}
		else
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags srcStage;
		VkPipelineStageFlags dstStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			dstStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else
			throw std::runtime_error("Unsupported Vulkan layout transition!");

		vkCmdPipelineBarrier(
			commandBuffer,
			srcStage, dstStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		vkData->Buffer.EndSingleCommand(commandBuffer);
	}

	static void CopyBufferToImage(VkBuffer buffer, VkImage image, ui32 width, ui32 height) {
		VkCommandBuffer commandBuffer = vkData->Buffer.BeginSingleCommand();

		VkBufferImageCopy region{};

		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = {
			width,
			height,
			1
		};

		vkCmdCopyBufferToImage(
			commandBuffer,
			buffer,
			image,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&region
		);

		vkData->Buffer.EndSingleCommand(commandBuffer);
	}

	/* ---------------- */

	void VulkanTexture::CreateTextureImage() {
		i32 texWidth, texHeight, texChanels;

		stbi_uc* pixels = stbi_load("assets/textures/pufferfish.png", &texWidth, &texHeight, &texChanels, STBI_rgb_alpha);

		VkDeviceSize imageSize = static_cast<VkDeviceSize>(texWidth) * texHeight * 4;

		if (!pixels)
			throw std::runtime_error("Failed to load texture image!");

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingMemory;

		vkData->Buffer.Create(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingMemory);

		void* data;
		vkMapMemory(vkData->Device.GetLogical(), stagingMemory, 0, imageSize, 0, &data);
			memcpy(data, pixels, static_cast<ui32>(imageSize));
		vkUnmapMemory(vkData->Device.GetLogical(), stagingMemory);

		stbi_image_free(pixels);

		CreateImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Image, m_ImageMemory);

		TransitionImageLayout(m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
			CopyBufferToImage(stagingBuffer, m_Image, static_cast<ui32>(texWidth), static_cast<ui32>(texHeight));
		TransitionImageLayout(m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		vkDestroyBuffer(vkData->Device.GetLogical(), stagingBuffer, nullptr);
		vkFreeMemory(vkData->Device.GetLogical(), stagingMemory, nullptr);
	}

	void VulkanTexture::CreateTextureImageView() {
		m_ImageView = VkUtils::CreateImageView(m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
	}

	void VulkanDepthTexture::Init() {
		CreateDepthResources();
	}

	void VulkanDepthTexture::Destory() const {
		vkDestroyImageView(vkData->Device.GetLogical(), m_ImageView, nullptr);

		vkDestroyImage(vkData->Device.GetLogical(), m_Image, nullptr);
		vkFreeMemory(vkData->Device.GetLogical(), m_ImageMemory, nullptr);
	}

	void VulkanDepthTexture::CreateDepthResources() {
		VkFormat format = FindDepthFormat();

		CreateImage(vkData->Swapchain.GetExtent().width, vkData->Swapchain.GetExtent().height, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Image, m_ImageMemory);
		m_ImageView = VkUtils::CreateImageView(m_Image, format, VK_IMAGE_ASPECT_DEPTH_BIT);

		TransitionImageLayout(m_Image, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
	}

	VkFormat VulkanDepthTexture::FindDepthFormat() {
		return FindSupportedFormat(
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
	}

}	// Ocean
