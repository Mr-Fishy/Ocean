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

		// TODO: Convert to use FixedArray for better memory usage.
		/**
		 * @brief A struct that contains information of what the Physical Vulkan Device supports for the swapchain.
		 */
		struct SwapChainSupportDetails;

		/**
		 * @brief A struct that represents the graphics and presentation queue. Uses std::optional for device selection.
		 */
		struct QueueFamilyIndices {
			std::optional<u16> GraphicsFamily;
			std::optional<u16> PresentFamily;

			static constexpr u8 UniqueFamilyCount = 2;

			b8 IsComplete() const {
				return GraphicsFamily.has_value() && PresentFamily.has_value();
			}

		};	// QueueFamilyIndices

		/**
		 * @brief The configuration struct for the Device class.
		 */
		struct DeviceConfig {
			Allocator* allocator;

			Renderer* renderer;

			void* windowPtr;

			DeviceConfig(Allocator* allocator, Renderer* renderer, void* window) : allocator(allocator), renderer(renderer), windowPtr(window) { }

		};	// DeviceInfo

		/**
		 * @brief The abstract Device class that manages the Vulkan Device and device selection. As well as the surface and command buffers.
		 */
		class Device {
		public:
			/**
			 * @brief Initializes the Device with the given config. This includes selecting and creating the Logical Vulkan Device.
			 * @param config - The configuration to use.
			 */
			void Init(DeviceConfig* config);
			/**
			 * @brief Shuts down the Device, destroying the Logical Vulkan Device and other rendering components.
			 */
			void Shutdown();

			/**
			 * @brief Queries and determines which family indices to use for rendering and presenting commands.
			 * @param device - The physical device to use.
			 * @return The QueueFamilyIndices struct.
			 */
			QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) const;

			/**
			 * @brief Clear's the command buffer at the give frame.
			 * @param frame - The current frame to use.
			 */
			void FlushCommandBuffer(u8 frame);
			// TODO: Confirm that imageIndex and frame are not the same.
			/**
			 * @brief Records a command buffer to submit to the queue.
			 * @param imageIndex 
			 * @param frame - The current frame to use.
			 */
			void RecordCommandBuffer(u32 imageIndex, u8 frame);
			/**
			 * @brief Submits a command buffer to the queue.
			 * @param syncObjects - The frame's SyncObjects struct (ie. semaphores and fences).
			 * @param frame - The current frame to use.
			 */
			void SubmitCommandBuffer(const SyncObjects& syncObjects, u8 frame);

			/**
			 * @return The Physical Vulkan Device.
			 */
			VkPhysicalDevice GetPhysical() const { return m_Physical; }
			/**
			 * @return The Logical Vulkan Device.
			 */
			VkDevice GetLogical() const { return m_Device; }

			/**
			 * @return The window surface that is being rendered to.
			 */
			VkSurfaceKHR GetSurface() const { return m_Surface; }
			/**
			 * @return The index of the presentation queue.
			 */
			VkQueue GetPresentationQueue() const { return m_PresentQueue; }

			/**
			 * @brief Sets the SwapChain pointer so that the device can submit commands.
			 * @param swapChain - The SwapChain of the Renderer.
			 */
			void SetSwapChain(SwapChain* swapChain) { p_SwapChain = swapChain; }

			/**
			 * @return The GLFW window handle.
			 */
			void* GetWindowHandle() const { return p_WindowHandle; }

		private:
			/**
			 * @brief Determines if the device is suitable for the application's requirements.
			 * @param device - The Physical Vulkan Device to check.
			 * @return True if the device is suitable, False otherwise.
			 */
			b8 IsDeviceSuitable(VkPhysicalDevice device);
			/**
			 * @brief Checks if the device supports the required extensions.
			 * @param device - The Physical Vulkan Device to check.
			 * @return True if the extensions are supported, False otherwise.
			 */
			b8 CheckDeviceExtensionSupport(VkPhysicalDevice device);

			/**
			 * @brief Creates the Logical Vulkan Device once the physical device has been chosen.
			 * @param indices - The queue indices that the renderer will submit commands to.
			 */
			void CreateLogicalDevice(QueueFamilyIndices indices);

			/**
			 * @brief Checks that the device supports a swapchain (ie. rendering a frame off screen and swapping it with the on-screen frame).
			 * @param device - The Physical Vulkan Device to check.
			 * @return The completed swapchain support details.
			 */
			SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device) const;

			/**
			 * @brief Creates the command pool that the renderer will submit commands through.
			 * @param indices - The queue indices to use for rendering and presenting.
			 */
			void CreateCommandPool(QueueFamilyIndices indices);
			/**
			 * @brief Creates the command buffers that will be used to record and submit commands.
			 */
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
