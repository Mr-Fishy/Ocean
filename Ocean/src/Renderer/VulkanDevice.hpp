#pragma once

#include "Ocean/Core/Types/ValueTypes.hpp"

#include "Ocean/Core/Primitives/Service.hpp"
#include "Ocean/Core/Primitives/Memory.hpp"
#include "Ocean/Core/Primitives/Array.hpp"

#include "Renderer/VulkanShader.hpp"
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

			VkPhysicalDevice GetPhysical() const { return m_Physical; }
			VkDevice GetLogical() const { return m_Device; }

		private:
			b8 IsDeviceSuitable(VkPhysicalDevice device);
			b8 CheckDeviceExtensionSupport(VkPhysicalDevice device);

			QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

			void CreateLogicalDevice(QueueFamilyIndices indices);

			SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

			VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
			VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availableModes);
			VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

			void CreateSwapChain();

			void CreateImageViews();

			void CreateRenderPass();

			void CreateGraphicsPipeline();

			/* --- */

			Allocator* p_Allocator = nullptr;
			Renderer* p_Renderer = nullptr;

			void* p_WindowHandle = nullptr;

			VkPhysicalDevice m_Physical = VK_NULL_HANDLE;
			VkDevice m_Device = VK_NULL_HANDLE;

			VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
			VkQueue m_PresentQueue = VK_NULL_HANDLE;

			VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;

			FixedArray<VkImage> m_SwapChainImages;
			VkFormat m_SwapChainFormat;
			VkExtent2D m_SwapChainExtent;
			FixedArray<VkImageView> m_SwapChainViews;

			Shader m_VertShader;
			Shader m_FragShader;
			// TODO: HashMap<cstring, Shader*> m_Shaders;

			VkRenderPass m_RenderPass = VK_NULL_HANDLE;
			VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
			VkPipeline m_GraphicsPipeline = VK_NULL_HANDLE;

			/* --- */

			static constexpr cstring k_DeviceExtensions[1] = {
				VK_KHR_SWAPCHAIN_EXTENSION_NAME
			};

		};

	}	// Vulkan

}	// Ocean
