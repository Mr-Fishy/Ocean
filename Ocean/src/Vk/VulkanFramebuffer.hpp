#pragma once

#include "Core/Types.hpp"

// libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Ocean {

	class VulkanFramebuffer {
	public:
		VulkanFramebuffer() = default;
		~VulkanFramebuffer() = default;

		void Init();
		void Destroy();

		std::vector<VkFramebuffer> GetBuffers() const { return v_Framebuffers; }

	private:
		void CreateFramebuffers();

		/* --- */

		std::vector<VkFramebuffer> v_Framebuffers;
	};

}	// Ocean
