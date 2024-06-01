#pragma once

// libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Ocean {

	class VulkanBuffer {
	public:
		VulkanBuffer() : m_CommandPool(VK_NULL_HANDLE) { }
		~VulkanBuffer() = default;

		void InitPool();
		void InitBuffers();
		void Destroy() const;

		void Create(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags props, VkBuffer& buffer, VkDeviceMemory& memory);
		void Copy(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

		VkCommandBuffer BeginSingleCommand() const;
		void EndSingleCommand(VkCommandBuffer commandBuffer) const;

		void RecordBuffer(VkCommandBuffer commandBuffer, ui32 imageIndex);

		VkCommandBuffer* GetBuffer(ui8 index) { return &v_CommandBuffers.at(index); }
		std::vector<VkCommandBuffer> GetBuffers() { return v_CommandBuffers; }

	private:
		void CreateCommandPool();

		void CreateCommandBuffer();

		/* --- */

		VkCommandPool m_CommandPool;

		std::vector<VkCommandBuffer> v_CommandBuffers;
	};

}	// Ocean
