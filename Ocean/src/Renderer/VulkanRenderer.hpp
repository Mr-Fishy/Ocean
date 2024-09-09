#pragma once

#include "Ocean/Core/Primitives/Service.hpp"
#include "Ocean/Core/Primitives/HashMap.hpp"

#include "Renderer/VulkanDevice.hpp"

namespace Ocean {

	class Window;

	namespace Vulkan {

		struct RendererConfig {
			Allocator* MemAllocator = nullptr;
			Window* MainWindow;

			cstring AppName;

			i16 AppVersionMajor;
			i16 AppVersionMinor;
			i16 AppVersionPatch;

		};	// VulkanRendererConfig

		class Renderer : public Service {
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

			/* --- */

			Allocator* p_Allocator = nullptr;

			VkInstance m_Instance = VK_NULL_HANDLE;
		#ifdef OC_DEBUG

			VkDebugUtilsMessengerEXT m_DebugMessenger = VK_NULL_HANDLE;

		#endif

			VkSurfaceKHR m_Surface = VK_NULL_HANDLE;

			Device* p_Device = nullptr;

			/* --- */

			const std::array<cstring, 1> k_ValidationLayers = {
				"VK_LAYER_KHRONOS_validation"
			};

		};	// Renderer

	}	// Vulkan

}	// Ocean
