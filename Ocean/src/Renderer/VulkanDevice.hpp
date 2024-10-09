#pragma once

#include "Ocean/Core/Types/ValueTypes.hpp"

#include "Ocean/Core/Primitives/Service.hpp"
#include "Ocean/Core/Primitives/Memory.hpp"
#include "Ocean/Core/Primitives/Array.hpp"

#include "Renderer/VulkanShader.hpp"

#include "Renderer/VulkanResources.hpp"

// std
#include <optional>

namespace Ocean {

	namespace Vulkan {

		class Renderer;
		class SwapChain;
		struct SyncObjects;



		struct SwapChainSupportDetails;

		struct QueueFamilyIndices {
			std::optional<u16> GraphicsFamily;
			std::optional<u16> PresentFamily;

			static constexpr u8 UniqueFamilyCount = 2;

			b8 IsComplete() const {
				return GraphicsFamily.has_value() && PresentFamily.has_value();
			}

		};	// QueueFamilyIndices

		struct DeviceConfig {
			Allocator* allocator;

			Renderer* renderer;

			void* windowPtr;

			DeviceConfig(Allocator* allocator, Renderer* renderer, void* window) : allocator(allocator), renderer(renderer), windowPtr(window) { }

		};	// DeviceInfo

		class Device {
		public:
			void Init(DeviceConfig* config);
			void Shutdown();

			// u32 GetMemoryType();

			// VkResult CreateBuffer();

			// void CopyBuffer();

			// b8 IsExtensionSupported(cstring extension);

			QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) const;

			void FlushCommandBuffer(u8 frame);
			void RecordCommandBuffer(u32 imageIndex, u8 frame);
			void SubmitCommandBuffer(const SyncObjects& syncObjects, u8 frame);

			VkPhysicalDevice GetPhysical() const { return m_Physical; }
			VkDevice GetLogical() const { return m_Device; }

			VkSurfaceKHR GetSurface() const { return m_Surface; }
			VkQueue GetPresentationQueue() const { return m_PresentQueue; }

			void SetSwapChain(SwapChain* swapChain) { p_SwapChain = swapChain; }

		private:
			b8 IsDeviceSuitable(VkPhysicalDevice device);
			b8 CheckDeviceExtensionSupport(VkPhysicalDevice device);

			void CreateLogicalDevice(QueueFamilyIndices indices);

			SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device) const;

			void CreateCommandPool(QueueFamilyIndices indices);
			void CreateCommandBuffers();

			/* --- */

			Allocator* p_Allocator = nullptr;
			Renderer* p_Renderer = nullptr;
			SwapChain* p_SwapChain = nullptr;

			void* p_WindowHandle = nullptr;

			VkPhysicalDevice m_Physical = VK_NULL_HANDLE;
			VkDevice m_Device = VK_NULL_HANDLE;

			VkSurfaceKHR m_Surface = VK_NULL_HANDLE;

			VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
			VkQueue m_PresentQueue = VK_NULL_HANDLE;

			VkCommandPool m_CommandPool = VK_NULL_HANDLE;
			FixedArray<VkCommandBuffer> m_CommandBuffers;

			/* --- */

			static constexpr cstring k_DeviceExtensions[1] = {
				VK_KHR_SWAPCHAIN_EXTENSION_NAME
			};

		};

	}	// Vulkan

}	// Ocean
