#pragma once

// libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// std
#include <optional>

namespace Ocean {

	class VulkanContext {
	private:
		struct QueueFamilyIndices {
			std::optional<ui32> GraphicsFamily;
			std::optional<ui32> PresentFamily;

			bool IsComplete() const {
				return GraphicsFamily.has_value() && PresentFamily.has_value();
			}
		};

		struct SwapChainSupportDetails {
			VkSurfaceCapabilitiesKHR Capabilities{};

			std::vector<VkSurfaceFormatKHR> Formats;
			std::vector<VkPresentModeKHR> PresentModes;
		};

	public:
		VulkanContext(GLFWwindow* window);
		~VulkanContext();

		void DrawFrame();

	private:
		void CreateInstance();

		b8 CheckValidationLayerSupport();

		std::vector<const char*> GetRequiredExtensions();

		void InitDebugMessenger();
		//
		void SetDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& info);
		//

		void CreateSurface();

		void PickPhysicalDevice();
		//
		b8 IsDeviceSuitable(VkPhysicalDevice device);
			//
			b8 CheckDeviceExtensionSupport(VkPhysicalDevice device);
			//
		ui8 RateDeviceSuitability(VkPhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		//

		void CreateLogicalDevice();

		void CreateSwapChain();
		//
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		//

		void CreateImageViews();

		void CreateRenderPass();

		void CreateGraphicsPipeline();
		//
		VkShaderModule CreateShaderModule(const std::vector<char>& code);
		//

		void CreateFrameBuffers();

		void CreateCommandPool();

		void CreateCommandBuffer();

		void CreateSyncObjects();

		void RecordCommandBuffer(VkCommandBuffer commandBuffer, ui32 imageIndex);

		/* --- */

		GLFWwindow* m_Window;

		VkInstance m_Instance;

		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;

		VkPhysicalDevice m_PhysicalDevice;
		VkDevice m_Device;

		VkSurfaceKHR m_Surface;

		VkSwapchainKHR m_SwapChain;
		std::vector<VkImage> v_SwapChainImages;

		VkFormat m_SwapChainImageFormat;
		VkExtent2D m_SwapChainExtent;
		
		std::vector<VkImageView> v_SwapChainImageViews;

		VkRenderPass m_RenderPass;
		VkPipelineLayout m_PipelineLayout;

		VkPipeline m_GraphicsPipeline;

		std::vector<VkFramebuffer> v_FrameBuffers;

		VkCommandPool m_CommandPool;

		VkCommandBuffer m_CommandBuffer;

		VkSemaphore m_ImageAvailableSemaphore;
		VkSemaphore m_RenderFinsishedSemaphore;
		VkFence m_InFlightFence;



		VkDebugUtilsMessengerEXT m_Messenger;

		const std::vector<const char*> c_ValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		const std::vector<const char*> c_DeviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};
	};

}	// Ocean
