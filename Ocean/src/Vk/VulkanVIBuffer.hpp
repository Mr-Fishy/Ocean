#pragma once

#include "Core/Types.hpp"

// libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Ocean {

	class VulkanVertexBuffer {
	public:
		VulkanVertexBuffer() : m_Buffer(VK_NULL_HANDLE), m_Memory(VK_NULL_HANDLE) { }
		~VulkanVertexBuffer() = default;

		void Init();
		void Destroy();

		VkBuffer GetBuffer() const { return m_Buffer; }

	private:
		void CreateVertexBuffer();

		/* --- */

		VkBuffer m_Buffer;
		VkDeviceMemory m_Memory;
	};

	class VulkanIndexBuffer {
	public:
		VulkanIndexBuffer() : m_Buffer(VK_NULL_HANDLE), m_Memory(VK_NULL_HANDLE) { }
		~VulkanIndexBuffer() = default;

		void Init();
		void Destroy() const;

		VkBuffer GetBuffer() const { return m_Buffer; }

		// TEMPORARY
		std::vector<ui16> GetIndices() const { return indices; }

	private:
		void CreateIndexBuffer();

		/* --- */

		VkBuffer m_Buffer;
		VkDeviceMemory m_Memory;

		// TEMPORARY
		const std::vector<ui16> indices = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};
	};

}	// Ocean
