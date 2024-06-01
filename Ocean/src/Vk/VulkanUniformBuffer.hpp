#pragma once

// libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Ocean {

	class VulkanUniformBuffer {
	public:
		VulkanUniformBuffer() = default;
		~VulkanUniformBuffer() = default;

		void Init();
		void Destroy();

		void Update(ui32 currentImage);

		std::vector<VkBuffer> GetBuffers() const { return v_Buffers; }

	private:
		void CreateUniformBuffers();

		/* --- */

		std::vector<VkBuffer> v_Buffers;
		std::vector<VkDeviceMemory> v_Memory;

		std::vector<void*> v_Mapped;
	};

}	// Ocean
