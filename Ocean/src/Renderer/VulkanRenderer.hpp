#pragma once

#include "Ocean/Core/Primitives/Service.hpp"
#include "Ocean/Core/Primitives/HashMap.hpp"

#include "Renderer/VulkanResources.hpp"

namespace Ocean {

	class Window;

	namespace Vulkan {

		class Device;
		class SwapChain;



		struct RendererConfig {
			Allocator* MemAllocator = nullptr;
			Window* MainWindow;

			cstring AppName;

			i16 AppVersionMajor;
			i16 AppVersionMinor;
			i16 AppVersionPatch;

		};	// VulkanRendererConfig

		class Renderer : public Service {
		private:
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

		public:
			OCEAN_DECLARE_SERVICE(Renderer);

			virtual void Init(void* config) override;
			virtual void Shutdown() override;

			void BeginFrame();
			void EndFrame();

			f32 AspectRatio() const;
			void ResizeSwapchain(u32 width, u32 height);

			VkInstance GetVulkanInstance() const { return m_Instance; }
			VkSurfaceKHR GetVulkanSurface() const { return m_Surface; }


			static cstring Name() { return "OCEAN_Rendering_Service"; }

		private:
			b8 CheckValidationSupport();

			std::vector<cstring> GetRequiredExtensions();

			void SetDebugMessengerInfo(VkDebugUtilsMessengerCreateInfoEXT& info);
			void CreateDebugMessenger();

			void CreateSyncObjects();

			void CreateRenderPass();

			void CreateGraphicsPipeline();

			/* --- */

			Allocator* p_Allocator = nullptr;

			VkInstance m_Instance = VK_NULL_HANDLE;

		#ifdef OC_DEBUG

			VkDebugUtilsMessengerEXT m_DebugMessenger = VK_NULL_HANDLE;

		#endif

			VkSurfaceKHR m_Surface = VK_NULL_HANDLE;

			Device* p_Device = nullptr;
			SwapChain* p_SwapChain = nullptr;

			SyncObjects m_SyncObjects;

			VkRenderPass m_RenderPass = VK_NULL_HANDLE;
			VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
			VkPipeline m_GraphicsPipeline = VK_NULL_HANDLE;

			FixedArray<Framebuffer> m_Framebuffers;

			VkCommandBuffer m_CommandBuffer = VK_NULL_HANDLE;

			/* --- */

			const std::array<cstring, 1> k_ValidationLayers = {
				"VK_LAYER_KHRONOS_validation"
			};

		};	// Renderer

	}	// Vulkan

}	// Ocean
