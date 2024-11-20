#include "Buffer.hpp"

#include "Renderer/Components/VkTypes.hpp"

#include "Renderer/Resources.hpp"
#include "Renderer/Device.hpp"
#include <vulkan/vulkan_core.h>

namespace Ocean {

	namespace Vulkan {

		void Buffer::Init(BufferConfig* config) {
			p_DeviceRef = config->device;

			VkDevice device = p_DeviceRef->GetLogical();

			Buffers::ExclusiveCreateInfo createInfo(
				config->size,
				config->usageFlags
			);

			CHECK_RESULT(
				vkCreateBuffer(device, &createInfo, nullptr, &m_Buffer),
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

			MemoryAllocation allocationInfo(
				memRequirements.size, 
				p_DeviceRef->GetMemoryType(memRequirements.memoryTypeBits, config->memoryFlags)
			);

			CHECK_RESULT(
				vkAllocateMemory(device, &allocationInfo, nullptr, &m_Memory),
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

			memcpy(ptr, data, static_cast<sizet>(size));

			vkUnmapMemory(p_DeviceRef->GetLogical(), m_Memory);
		}



		void* UniformBuffer::GetMappedMemory(sizet size) {
			void* ptr = nullptr;

			vkMapMemory(p_DeviceRef->GetLogical(), m_Memory, 0, size, 0, &ptr);

			return ptr;
		}

	}	// Vulkan

}	// Ocean
