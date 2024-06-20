#include "ocpch.hpp"

#include "Platform/Vk/VulkanVertexIndexBuffer.hpp"

#include <vulkan/vulkan.hpp>

namespace Ocean::Vk {

	struct Vertex {
		vec2 pos;
		vec3 color;

		static VkVertexInputBindingDescription GetBindingDescription() {
			VkVertexInputBindingDescription desc{};

			desc.binding = 0;
			desc.stride = sizeof(Vertex);
			desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return desc;
		}

		static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 2> attributes{};

			attributes[0].binding = 0;
			attributes[0].location = 0;
			attributes[0].format = VK_FORMAT_R32G32_SFLOAT;
			attributes[0].offset = offsetof(Vertex, pos);

			attributes[1].binding = 0;
			attributes[1].location = 1;
			attributes[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributes[1].offset = offsetof(Vertex, color);

			return attributes;
		}
	};

	VulkanVertexBuffer::VulkanVertexBuffer(ui32 size) {

	}

	VulkanVertexBuffer::VulkanVertexBuffer(f32* vertices, ui32 size) {
	}

	VulkanVertexBuffer::~VulkanVertexBuffer() {
	}

	void VulkanVertexBuffer::Bind() const {
	}

	void VulkanVertexBuffer::Unbind() const {
	}

	void VulkanVertexBuffer::SetData(const void* data, ui32 size) {
	}

	VulkanIndexBuffer::VulkanIndexBuffer(ui32* indices, ui32 count) : m_Count(count) {
	}

	VulkanIndexBuffer::~VulkanIndexBuffer() {
	}

	void VulkanIndexBuffer::Bind() const {
	}

	void VulkanIndexBuffer::Unbind() const {
	}

}	// Ocean::Vk
