#include "VulkanRenderer.hpp"

#include "Ocean/Core/Window.hpp"

#include <GLFW/glfw3.h>

namespace Ocean {

	namespace Vulkan {

		Renderer s_Renderer;

		Renderer* Renderer::Instance() {
			return &s_Renderer;
		}

		void Renderer::Init(void* config) {
			RendererConfig* renConfig = (RendererConfig*)config;
			p_Allocator = renConfig->MemAllocator;

			VkApplicationInfo appInfo{ };
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

			appInfo.pApplicationName = renConfig->AppName;
			appInfo.applicationVersion = VK_MAKE_API_VERSION(1, renConfig->AppVersionMajor, renConfig->AppVersionMinor, renConfig->AppVersionPatch);

			appInfo.pEngineName = "Ocean Engine";
			appInfo.engineVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);

			appInfo.apiVersion = VK_API_VERSION_1_3;

			VkInstanceCreateInfo createInfo{ };
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;

			auto extensions = GetRequiredExtensions();
			createInfo.enabledExtensionCount = (u32)extensions.size();
			createInfo.ppEnabledExtensionNames = extensions.data();

		#ifdef OC_DEBUG

			OASSERTM(CheckValidationSupport(), "Validation layers requested, but not available!");

			createInfo.enabledLayerCount = k_ValidationLayers.size();
			createInfo.ppEnabledLayerNames = k_ValidationLayers.data();

			VkDebugUtilsMessengerCreateInfoEXT debugInfo{ };
			SetDebugMessengerInfo(debugInfo);

			createInfo.pNext = &debugInfo;

		#else

			createInfo.enabledLayerCount = 0;
			createInfo.ppEnabledLayerNames = nullptr;

			createInfo.pNext = nullptr;

		#endif

			CheckResultSuccess(
				vkCreateInstance(&createInfo, nullptr, &m_Instance),
				"Failed to create instance!");

		#ifdef OC_DEBUG

			CreateDebugMessenger();

		#endif

			CheckResultSuccess(
				glfwCreateWindowSurface(m_Instance, (GLFWwindow*)renConfig->MainWindow->Handle(), nullptr, &m_Surface),
				"Failed to create window surface!");

			DeviceConfig devConfig{ p_Allocator, this, renConfig->MainWindow->Handle() };
			p_Device = (Device*)p_Allocator->Allocate(sizeof(Device), alignof(Device));
			p_Device->Init(&devConfig);
		}

		void Renderer::Shutdown() {
			p_Device->Shutdown();
			p_Allocator->Deallocate(p_Device);

		#ifdef OC_DEBUG

			DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);

		#endif

			vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
			vkDestroyInstance(m_Instance, nullptr);
		}

		void Renderer::BeginFrame() {
		}

		void Renderer::EndFrame() {
		}

		f32 Renderer::AspectRatio() const {
			return f32();
		}

		void Renderer::ResizeSwapchain(u32 width, u32 height) {
		}

		b8 Renderer::CheckValidationSupport() {
			u32 layerCount;
			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

			std::vector<VkLayerProperties> availableLayers(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

			for (const char* layerName : k_ValidationLayers) {
				b8 layerFound = false;

				for (const auto& layerProperties : availableLayers) {
					if (strcmp(layerName, layerProperties.layerName) == 0) {
						layerFound = true;
						break;
					}
				}

				if (!layerFound) {
					return false;
				}
			}

			return true;
		}

		std::vector<cstring> Renderer::GetRequiredExtensions() {
			u32 glfwExtensionCount = 0;
			cstring* glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			std::vector<cstring> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		#ifdef OC_DEBUG

			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		#endif

			return extensions;
		}

		void Renderer::SetDebugMessengerInfo(VkDebugUtilsMessengerCreateInfoEXT& info) {
			info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

			info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

			info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

			info.pfnUserCallback = oDebugCallback;
			info.pUserData = nullptr; // Optional
		}

		void Renderer::CreateDebugMessenger() {
			VkDebugUtilsMessengerCreateInfoEXT info{ };
			SetDebugMessengerInfo(info);

			CheckResultSuccess(
				CreateDebugUtilsMessengerEXT(m_Instance, &info, nullptr, &m_DebugMessenger),
				"Failed to set up debug messenger!");
		}

	}	// Vulkan

}	// Ocean
