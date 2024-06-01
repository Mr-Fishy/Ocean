#include "ocpch.hpp"

#include "Vk/VulkanVIBuffer.hpp"

#include "Vk/VulkanData.hpp"

namespace Ocean {

	void VulkanVertexBuffer::Init() {
		CreateVertexBuffer();
	}

	void VulkanVertexBuffer::Destroy() {
		vkDestroyBuffer(vkData->Device.GetLogical(), m_Buffer, nullptr);
		vkFreeMemory(vkData->Device.GetLogical(), m_Memory, nullptr);
	}

	// TEMPORARY
	const std::vector<Vertex> vertices = {
		{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
		{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

		{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
		{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
	};

	void VulkanVertexBuffer::CreateVertexBuffer() {
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory  stagingMemory;
		vkData->Buffer.Create(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingMemory);

		void* data;
		vkMapMemory(vkData->Device.GetLogical(), stagingMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), static_cast<ui64>(bufferSize));
		vkUnmapMemory(vkData->Device.GetLogical(), stagingMemory);

		vkData->Buffer.Create(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Buffer, m_Memory);

		vkData->Buffer.Copy(stagingBuffer, m_Buffer, bufferSize);

		vkDestroyBuffer(vkData->Device.GetLogical(), stagingBuffer, nullptr);
		vkFreeMemory(vkData->Device.GetLogical(), stagingMemory, nullptr);
	}

	void VulkanIndexBuffer::Init() {
		CreateIndexBuffer();
	}

	void VulkanIndexBuffer::Destroy() const {
		vkDestroyBuffer(vkData->Device.GetLogical(), m_Buffer, nullptr);
		vkFreeMemory(vkData->Device.GetLogical(), m_Memory, nullptr);
	}

	void VulkanIndexBuffer::CreateIndexBuffer() {
		VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingMemory;
		vkData->Buffer.Create(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingMemory);

		void* data;
		vkMapMemory(vkData->Device.GetLogical(), stagingMemory, 0, bufferSize, 0, &data);
		memcpy(data, indices.data(), static_cast<ui64>(bufferSize));
		vkUnmapMemory(vkData->Device.GetLogical(), stagingMemory);

		vkData->Buffer.Create(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Buffer, m_Memory);

		vkData->Buffer.Copy(stagingBuffer, m_Buffer, bufferSize);

		vkDestroyBuffer(vkData->Device.GetLogical(), stagingBuffer, nullptr);
		vkFreeMemory(vkData->Device.GetLogical(), stagingMemory, nullptr);
	}

}	// Ocean
