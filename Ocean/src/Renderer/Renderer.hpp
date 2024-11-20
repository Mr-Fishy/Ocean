#pragma once

#include "Ocean/Core/Types/FloatingPoints.hpp"

#include "Ocean/Core/Primitives/Service.hpp"
#include "Ocean/Core/Primitives/Array.hpp"

#include "Renderer/Components/Buffer.hpp"

// libs
#include <vulkan/vulkan_core.h>

// std
#include <array>
#include <vector>

namespace Ocean {

	class Window;

	namespace Vulkan {

		class Device;
		class SwapChain;
		class UniformBuffer;


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
		 * VertexBuffer vertexbuffer                            - Owned by Device
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



		/**
		 * @brief The syncronization objects for the renderer. AKA the presentation and rendering semaphore, as well as the frame in flight fence.
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

		/**
		 * @brief The configuration of the renderer.
		 */
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
		 * @brief The primary renderer class that will be interacted with by the application.
		 */
		class Renderer : public Service {
		private:
			struct UniformData {
				UniformBuffer UBO;
				void* Data;
			};

		public:
		#ifdef OC_DEBUG

			Renderer() : p_Allocator(nullptr), m_Instance(VK_NULL_HANDLE), m_DebugMessenger(VK_NULL_HANDLE), p_Device(nullptr), p_SwapChain(nullptr), m_RenderPass(VK_NULL_HANDLE), m_PipelineLayout(VK_NULL_HANDLE), m_DescriptorSetLayout(VK_NULL_HANDLE), m_GraphicsPipeline(VK_NULL_HANDLE), m_SyncObjects(), m_UniformBuffers(), m_Width(0), m_Height(0), m_MaxFramesInFlight(2), m_Frame(0), m_VulkanFrame(0) { }

		#else

			Renderer() : p_Allocator(nullptr), m_Instance(VK_NULL_HANDLE), p_Device(nullptr), p_SwapChain(nullptr), m_RenderPass(VK_NULL_HANDLE), m_PipelineLayout(VK_NULL_HANDLE), m_DescriptorSetLayout(VK_NULL_HANDLE), m_GraphicsPipeline(VK_NULL_HANDLE), m_SyncObjects(), m_UniformBuffers(), m_Width(0), m_Height(0), m_MaxFramesInFlight(2), m_Frame(0), m_VulkanFrame(0) { }
		
		#endif
			~Renderer() = default;

			static Renderer& Instance();

			/**
			 * @brief Initializes the Renderer with the given configuration.
			 * @param config - The renderer configuration to use.
			 */
			void Init(RendererConfig* config);
			/**
			 * @brief Shuts down the Renderer.
			 */
			static void Shutdown();

		private:
			void IntermediateShutdown();

		public:

			/**
			 * @brief Begin's the frame rendering.
			 */
			void BeginFrame();
			/**
			 * @brief Render's the current frame.
			 */
			void RenderFrame();
			/**
			 * @brief End's the frame rendering.
			 */
			void EndFrame();

			/**
			 * @brief Clean's up the GPU operations before closing. This must be called before exiting the runtime.
			 */
			void CleanUp();

			/**
			 * @return The aspect ratio of the Renderer's output image.
			 */
			f32 AspectRatio() const;
			/**
			 * @brief Resizes the swapchain to the given width and height.
			 * @param width - The new width of the swapchain.
			 * @param height - The new height of the swapchain.
			 */
			void ResizeSwapchain(i32 width, i32 height);

			/**
			 * @return The Renderer's Vulkan instance.
			 */
			VkInstance GetVulkanInstance() const { return m_Instance; }
			/**
			 * @return The Renderer's Vulkan Render Pass.
			 */
			VkRenderPass GetRenderPass() const { return m_RenderPass; }
			VkPipelineLayout GetPipelineLayout() const { return m_PipelineLayout; }
			/**
			 * @return The Renderer's Vulkan Graphics Pipeline.
			 */
			VkPipeline GetGraphicsPipeline() const { return m_GraphicsPipeline; }

			VkDescriptorSetLayout GetDescriptorLayout() const { return m_DescriptorSetLayout; }

			VkBuffer GetUniformBuffer(u8 frame) const { return m_UniformBuffers.Get(frame).UBO.GetBuffer(); }

			/**
			 * @return The maximum frames in flight the Renderer has.
			 */
			u8 GetMaxFramesInFlight() const { return m_MaxFramesInFlight; }

			/**
			 * @return The Renderer's service name.
			 */
			static cstring Name() { return "OCEAN_Rendering_Service"; }

		private:
			Renderer(const Renderer&) = delete;
			Renderer& operator = (const Renderer&) = delete;

			/**
			 * @brief Checks if the system's Vulkan library support's the layers defined in k_ValidationLayers.
			 * @return True if supported, False otherwise.
			 */
			b8 CheckValidationSupport();

			/**
			 * @brief Get's the required extensions to enable for the Vulkan Instance. 
			 * @return A vector of the extension names.
			 */
			std::vector<cstring> GetRequiredExtensions();

		#ifdef OC_DEBUG

			/**
			 * @brief Sets the information for the Vulkan Debug Messenger to the given info.
			 * @param info - The Debug Messenger info.
			 */
			void SetDebugMessengerInfo(VkDebugUtilsMessengerCreateInfoEXT& info);
			/**
			 * @brief Creates the Vulkan Debug Messenger so that the validation layers will report errors.
			 */
			void CreateDebugMessenger();

		#endif

			/**
			 * @brief Creates the Vulkan Render Pass for the Graphics Pipeline. 
			 */
			void CreateRenderPass();

			/**
			 * @brief 
			 */
			void CreateDescriptorSetLayout();

			/**
			 * @brief Creates the Vulkan Graphics Pipeline so that the GPU knows how to render (or compute).
			 */
			void CreateGraphicsPipeline();

			/**
			 * @brief Creates the Vulkan Semaphores and Vulkan Fences that are needed for the runtime.
			 */
			void CreateSyncObjects();

			void CreateUniformBuffers();

			void UpdateUniformBuffer(u8 frame);

			/* --- */

			static inline Renderer* s_Instance = nullptr;

			Allocator* p_Allocator;

			VkInstance m_Instance;

		#ifdef OC_DEBUG

			VkDebugUtilsMessengerEXT m_DebugMessenger;

		#endif

			Device* p_Device;
			SwapChain* p_SwapChain;

			VkRenderPass m_RenderPass;

			VkPipelineLayout m_PipelineLayout;
			VkDescriptorSetLayout m_DescriptorSetLayout;
			VkPipeline m_GraphicsPipeline;

			FixedArray<SyncObjects> m_SyncObjects;

			FixedArray<UniformData> m_UniformBuffers;

			u16 m_Width;
			u16 m_Height;

			u8 m_MaxFramesInFlight;
			u8 m_Frame;

			u32 m_VulkanFrame;

			/* --- */

			const std::array<cstring, 1> k_ValidationLayers = {
				"VK_LAYER_KHRONOS_validation"
			};

		};	// Renderer

	}	// Vulkan

}	// Ocean
