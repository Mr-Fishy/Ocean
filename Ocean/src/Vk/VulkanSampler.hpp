#pragma once

// libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Ocean {

	class VulkanSampler {
	public:
		VulkanSampler() : m_TextureSampler(VK_NULL_HANDLE) { }
		~VulkanSampler() = default;

		void Init();
		void Destroy();

		VkSampler GetTextureSampler() const { return m_TextureSampler; }

	private:
		void CreateTextureSampler();

		/* --- */

		VkSampler m_TextureSampler;
	};

}	// Ocean
