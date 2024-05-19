#pragma once

#include "Core/Types.hpp"

// libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// std
#include <optional>

namespace Ocean {

	// TODO: Seperate to different classes for easier management

	struct VulkanData {
		VkInstance Instance;

		VkPhysicalDevice PhysicalDevice;
		VkDevice Device;

		VkSurfaceKHR Surface;

		VkQueue GraphicsQueue;
		VkQueue PresentQueue;

		VkBuffer VertexBuffer;
		VkDeviceMemory VertexBufferMemory;

		VkBuffer IndexBuffer;
		VkDeviceMemory IndexBufferMemory;

		std::vector<VkBuffer> UniformBuffers;
		std::vector<VkDeviceMemory> UniformBuffersMemory;
		std::vector<void*> UniformBuffersMapped;

		VkDescriptorPool DescriptorPool;
		std::vector<VkDescriptorSet> DescriptorSets;
	};

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
				std::array<VkVertexInputAttributeDescription, 2> desc{};

				desc[0].binding = 0;
				desc[0].location = 0;
				desc[0].format = VK_FORMAT_R32G32_SFLOAT;
				desc[0].offset = offsetof(Vertex, pos);

				desc[1].binding = 0;
				desc[1].location = 1;
				desc[1].format = VK_FORMAT_R32G32B32_SFLOAT;
				desc[1].offset = offsetof(Vertex, color);

				return desc;
			}
		};

		struct UniformBufferObject {
			mat4 model;
			mat4 view;
			mat4 proj;
		};

	public:
		VulkanContext(GLFWwindow* window);
		~VulkanContext();

		void DrawFrame();

		void FramebufferResized() { m_FramebufferResized = true; }

		VulkanData* GetContext() { return m_Data; }
		VkDevice* GetDevice() { return &m_Data->Device; }

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

		void CreateDescriptorSetLayout();

		void CreateGraphicsPipeline();
		//
		VkShaderModule CreateShaderModule(const std::vector<char>& code);
		//

		void CreateFrameBuffers();

		void CreateCommandPool();

		void CreateVertexBuffer();
		//
		void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags props, VkBuffer& buffer, VkDeviceMemory& mem);
			//
			ui32 FindMemoryType(ui32 typeFilter, VkMemoryPropertyFlags properties);
			//
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		//

		void CreateIndexBuffer();

		void CreateUniformBuffers();
		//
		void UpdateUniformBuffer(ui32 currentImage);
		//

		void CreateDescriptorPool();

		void CreateDescriptorSets();

		void CreateCommandBuffers();

		void CreateSyncObjects();

		void RecordCommandBuffer(VkCommandBuffer commandBuffer, ui32 imageIndex);

		void CleanSwapChain();
		void RecreateSwapChain();

		/* --- */

		GLFWwindow* m_Window;

		VulkanData* m_Data;

		VkSwapchainKHR m_SwapChain;
		std::vector<VkImage> v_SwapChainImages;

		VkFormat m_SwapChainImageFormat;
		VkExtent2D m_SwapChainExtent;
		
		std::vector<VkImageView> v_SwapChainImageViews;

		VkRenderPass m_RenderPass;

		VkDescriptorSetLayout m_DescriptorSetLayout;
		VkPipelineLayout m_PipelineLayout;

		VkPipeline m_GraphicsPipeline;

		std::vector<VkFramebuffer> v_FrameBuffers;

		VkCommandPool m_CommandPool;

		std::vector<VkCommandBuffer> v_CommandBuffers;

		std::vector<VkSemaphore> v_ImageAvailableSemaphores;
		std::vector<VkSemaphore> v_RenderFinsishedSemaphores;
		std::vector<VkFence> v_InFlightFences;
		
		b8 m_FramebufferResized = false;

		ui8 m_CurrentFrame = 0;



		VkDebugUtilsMessengerEXT m_Messenger;

		const std::vector<const char*> c_ValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		const std::vector<const char*> c_DeviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		const ui8 MAX_FRAMES_IN_FLIGHT = 2;


		// Temp
		const std::vector<Vertex> vertices = {
			{ {-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f} },
			{ { 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f} },
			{ { 0.5f,  0.5f}, {0.0f, 0.0f, 1.0f} },
			{ {-0.5f,  0.5f}, {1.0f, 1.0f, 1.0f} }
		};

		const std::vector<ui16> indices = {
			0, 1, 2, 2, 3, 0
		};
	};

}	// Ocean
