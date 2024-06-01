#include "ocpch.hpp"

#include "Vk/VulkanSampler.hpp"

#include "Vk/VulkanData.hpp"

namespace Ocean {

	void VulkanSampler::Init() {
		CreateTextureSampler();
	}
		 
	void VulkanSampler::Destroy() {
		vkDestroySampler(vkData->Device.GetLogical(), m_TextureSampler, nullptr);
	}
		 
	void VulkanSampler::CreateTextureSampler() {
		VkPhysicalDeviceProperties props{};
		vkGetPhysicalDeviceProperties(vkData->Device.GetPhysical(), &props);

		VkSamplerCreateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

		info.magFilter = VK_FILTER_LINEAR;
		info.minFilter = VK_FILTER_LINEAR;

		info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

		info.anisotropyEnable = VK_FALSE;
		info.maxAnisotropy = 1.0f;

		info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

		info.unnormalizedCoordinates = VK_FALSE;

		info.compareEnable = VK_FALSE;
		info.compareOp = VK_COMPARE_OP_ALWAYS;

		info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

		if (vkCreateSampler(vkData->Device.GetLogical(), &info, nullptr, &m_TextureSampler) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan texture sampler!");
	}

}	// Ocean
