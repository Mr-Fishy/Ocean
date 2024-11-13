#pragma once

#include "Renderer/Resources.hpp"

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
			Buffer() = default;
			~Buffer() = default;

			void Init(BufferConfig* config);
			void Shutdown();

			void SubmitData(sizet size, void* data);

			VkBuffer GetBuffer() const { return m_Buffer; }

		protected:
			Device* p_DeviceRef = nullptr;

			VkBuffer m_Buffer = VK_NULL_HANDLE;
			VkDeviceMemory m_Memory = VK_NULL_HANDLE;

		};	// Buffer



		class UniformBuffer : public Buffer {
		public:
			UniformBuffer() = default;
			~UniformBuffer() = default;

			void* GetMappedMemory(sizet size);

		};	// Uniform Buffer

	}	// Vulkan

}	// Ocean
