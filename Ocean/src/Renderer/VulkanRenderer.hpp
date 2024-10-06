#pragma once

#include "Ocean/Core/Primitives/Service.hpp"
#include "Ocean/Core/Primitives/Array.hpp"

#include "Renderer/VulkanResources.hpp"

namespace Ocean {

	class Window;

	namespace Vulkan {

		class Device;
		class SwapChain;


		/**
		 * @page vkRenderer Vulkan Renderer
		 * @section vkItems Overall Vulkan Members
		 * 
		 * VkInstance instance;                                 - Owned by Renderer
		 * VkDebugUtilsMessengerEXT debugMessenger;             - Owned by Renderer
		 * VkRenderPass renderPass;                             - Owned by Renderer
		 * VkPipelineLayout pipelineLayout;                     - Owned by Renderer
		 * VkPipeline graphicsPipeline;                         - Owned by Renderer
		 * VkSemaphore presentSemaphore;                        - Owned by Renderer
		 * VkSemaphore renderSemaphore;                         - Owned by Renderer
		 * VkFence inFlightFence;                               - Owned by Renderer
		 * 
		 * VkPhysicalDevice physicalDevice;                     - Owned by Device
		 * VkDevice device;                                     - Owned by Device
		 * VkSurfaceKHR surface;                                - Owned by Device
		 * VkQueue graphicsQueue;                               - Owned by Device
		 * VkQueue presentQueue;                                - Owned by Device
		 * VkCommandPool commandPool;                           - Owned by Device
		 * FixedArray<VkCommandBuffer> commandBuffer;           - Owned by Device
		 * 
		 * VkSwapchainKHR swapChain;                            - Owned by SwapChain
		 * FixedArray<VkImage> swapChainImages;                 - Owned by SwapChain
		 * VkFormat swapChainImageFormat;                       - Owned by SwapChain
		 * VkExtent2D swapChainExtent;                          - Owned by SwapChain
		 * FixedArray<VkImageView> swapChainImageViews;         - Owned by SwapChain
		 * FixedArray<VkFramebuffer> swapChainFramebuffers;     - Owned by SwapChain
		 * 
		 */



		struct SyncObjects {
			struct _Sempahores {
				VkSemaphore PresentComplete;

				VkSemaphore RenderComplete;
			};

			struct _Fences {
				VkFence InFlight;
			};

			_Sempahores Sempahores;
			_Fences Fences;
		};

		struct RendererConfig {
			Allocator* MemAllocator = nullptr;
			Window* MainWindow;

			cstring AppName;

			u16 Width;
			u16 Height;

			u16 MaxFramesInFlight;

			i16 AppVersionMajor;
			i16 AppVersionMinor;
			i16 AppVersionPatch;

		};	// VulkanRendererConfig

		/**
		 * @brief 
		 */
		class Renderer : public Service {
		public:
			OCEAN_DECLARE_SERVICE(Renderer);

			virtual void Init(void* config) override;
			virtual void Shutdown() override;

			void BeginFrame();
			void RenderFrame();
			void EndFrame();

			void CleanUp();

			f32 AspectRatio() const;
			void ResizeSwapchain(u32 width, u32 height);

			VkInstance GetVulkanInstance() const { return m_Instance; }
			VkRenderPass GetRenderPass() const { return m_RenderPass; }
			VkPipeline GetGraphicsPipeline() const { return m_GraphicsPipeline; }

			static cstring Name() { return "OCEAN_Rendering_Service"; }

		private:
			b8 CheckValidationSupport();

			std::vector<cstring> GetRequiredExtensions();

			void SetDebugMessengerInfo(VkDebugUtilsMessengerCreateInfoEXT& info);
			void CreateDebugMessenger();

			void CreateRenderPass();

			void CreateGraphicsPipeline();

			void CreateSyncObjects();

			/* --- */

			Allocator* p_Allocator = nullptr;

			VkInstance m_Instance = VK_NULL_HANDLE;

		#ifdef OC_DEBUG

			VkDebugUtilsMessengerEXT m_DebugMessenger = VK_NULL_HANDLE;

		#endif

			Device* p_Device = nullptr;
			SwapChain* p_SwapChain = nullptr;

			VkRenderPass m_RenderPass = VK_NULL_HANDLE;

			VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
			VkPipeline m_GraphicsPipeline = VK_NULL_HANDLE;

			SyncObjects m_SyncObjects;

			u16 m_Width = 0;
			u16 m_Height = 0;

			u16 m_MaxFramesInFlight = 2;

			/* --- */

			const std::array<cstring, 1> k_ValidationLayers = {
				"VK_LAYER_KHRONOS_validation"
			};

		};	// Renderer

	}	// Vulkan

}	// Ocean
