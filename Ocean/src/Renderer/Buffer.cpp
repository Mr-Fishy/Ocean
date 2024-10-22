#include "Buffer.hpp"

#include "Renderer/Device.hpp"

namespace Ocean {

	namespace Vulkan {

		void Buffer::Init(BufferConfig* config) {
			p_DeviceRef = config->device;

			VkDevice device = p_DeviceRef->GetLogical();

			VkBufferCreateInfo info{ };
			info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			// Specifies the size of the buffer in bytes.
			info.size = config->size;
			// Specifies the usage of the buffer.
			info.usage = config->usageFlags;
			// Specifies that the sharing mode is exclusive to a single queue family.
			info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			// Used to configure sparse buffer memory.
			info.flags = 0;

			CHECK_RESULT(
				vkCreateBuffer(device, &info, nullptr, &m_Buffer),
				"Failed to create vertex buffer!"
			);

			/**
			 * VkMemoryRequirements:
			 * size: The size of the required amount of memory in bytes, this may differ from bufferInfo.size.
			 *
			 * alignment: The offset in bytes where the buffer begins in the allocated memory, depends on bufferIno.usage and bufferInfo.flags.
			 *
			 * memoryTypeBits: Bit field of the memory types that are suitable for the buffer.
			 */
			VkMemoryRequirements memRequirements;
			vkGetBufferMemoryRequirements(device, m_Buffer, &memRequirements);

			VkMemoryAllocateInfo allocInfo{ };
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

			allocInfo.allocationSize = memRequirements.size;

			allocInfo.memoryTypeIndex = p_DeviceRef->GetMemoryType(memRequirements.memoryTypeBits, config->memoryFlags);

			CHECK_RESULT(
				vkAllocateMemory(device, &allocInfo, nullptr, &m_Memory),
				"Failed to allocate vertex buffer memory!"
			);

			vkBindBufferMemory(device, m_Buffer, m_Memory, 0);
		}

		void Buffer::Shutdown() {
			vkDestroyBuffer(p_DeviceRef->GetLogical(), m_Buffer, nullptr);

			vkFreeMemory(p_DeviceRef->GetLogical(), m_Memory, nullptr);
		}

		void Buffer::SubmitData(sizet size, void* data) {
			void* ptr = nullptr;
			vkMapMemory(p_DeviceRef->GetLogical(), m_Memory, 0, size, 0, &ptr);

			memcpy(ptr, data, (sizet)size);

			vkUnmapMemory(p_DeviceRef->GetLogical(), m_Memory);
		}

		void* UniformBuffer::GetMappedMemory(sizet size) {
			void* ptr = nullptr;

			vkMapMemory(p_DeviceRef->GetLogical(), m_Memory, 0, size, 0, &ptr);

			return ptr;
		}

	}	// Vulkan

}	// Ocean
