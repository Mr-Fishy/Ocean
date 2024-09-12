#pragma once

#include "Ocean/Core/Types/ValueTypes.hpp"

#include "Ocean/Core/Primitives/Service.hpp"
#include "Ocean/Core/Primitives/Memory.hpp"
#include "Ocean/Core/Primitives/Array.hpp"

#include "Renderer/VulkanShader.hpp"
#include "Renderer/VulkanFramebuffer.hpp"
#include "Renderer/VulkanResources.hpp"

namespace Ocean {

	namespace Vulkan {

		class Renderer;


		struct QueueFamilyIndices;
		struct SwapChainSupportDetails;

		struct DeviceConfig {
			Allocator* MemAllocator;

			Renderer* Renderer;

			void* WindowPtr;

		};	// DeviceInfo

		class Device {
		public:
			void Init(DeviceConfig* config);
			void Shutdown();

			u32 GetMemoryType();

			VkResult CreateBuffer();

			void CopyBuffer();

			VkCommandPool CreateCommandPool(u32 queueFamilyIndex, VkCommandPoolCreateFlags flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

			VkCommandBuffer CreateCommandBuffer();
			void FlushCommandBuffer();

			b8 IsExtensionSupported(cstring extension);

			VkPhysicalDevice GetPhysical() const { return m_Physical; }
			VkDevice GetLogical() const { return m_Device; }

		private:
			b8 IsDeviceSuitable(VkPhysicalDevice device);
			b8 CheckDeviceExtensionSupport(VkPhysicalDevice device);

			QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

			void CreateLogicalDevice(QueueFamilyIndices indices);

			SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);



			void CreateCommandPool(QueueFamilyIndices indices);


			void RecordCommandBuffer(VkCommandBuffer, u32 imageIndex);

			/* --- */

			Allocator* p_Allocator = nullptr;
			Renderer* p_Renderer = nullptr;

			void* p_WindowHandle = nullptr;

			VkPhysicalDevice m_Physical = VK_NULL_HANDLE;
			VkDevice m_Device = VK_NULL_HANDLE;

			VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
			VkQueue m_PresentQueue = VK_NULL_HANDLE;

			Shader m_VertShader;
			Shader m_FragShader;
			// TODO: HashMap<cstring, Shader*> m_Shaders;

			VkCommandPool m_CommandPool = VK_NULL_HANDLE;

			/* --- */

			static constexpr cstring k_DeviceExtensions[1] = {
				VK_KHR_SWAPCHAIN_EXTENSION_NAME
			};

		};

	}	// Vulkan

}	// Ocean
