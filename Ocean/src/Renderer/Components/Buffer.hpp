#pragma once

#include "Ocean/Core/Types/Integers.hpp"

// libs
#include <vulkan/vulkan_core.h>

namespace Ocean {

	namespace Vulkan {

		class Device;



		struct BufferConfig {
			sizet size;

			VkBufferUsageFlags usageFlags;
			VkMemoryPropertyFlags memoryFlags;

			Device* device;
		};

		class Buffer {
		public:
			Buffer() : p_DeviceRef(nullptr), m_Buffer(VK_NULL_HANDLE), m_Memory(VK_NULL_HANDLE) { }
			~Buffer() = default;

			void Init(BufferConfig* config);
			void Shutdown();

			void SubmitData(sizet size, void* data);

			VkBuffer GetBuffer() const { return m_Buffer; }

		protected:
			Device* p_DeviceRef;

			VkBuffer m_Buffer;
			VkDeviceMemory m_Memory;

		};	// Buffer



		class UniformBuffer : public Buffer {
		public:
			UniformBuffer() : Buffer() { }
			~UniformBuffer() = default;

			void* GetMappedMemory(sizet size);

		};	// Uniform Buffer

	}	// Vulkan

}	// Ocean
