#include "ocpch.hpp"

#include "Vk/VulkanInstance.hpp"

#include "Vk/VulkanData.hpp"
#include "Ocean/Logger/Log.hpp"

namespace Ocean {

	/* DEBUG INFO */

	static VkResult CreateDebugUtilsMessengerEXT(
		VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pInfo,
		const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger) {

		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
			return func(instance, pInfo, pAllocator, pMessenger);

		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	static void DestroyDebugUtilsMessengerEXT(
		VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator) {

		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {

		std::stringstream ss;

		ss << "Vulkan |: " << pCallbackData->pMessage;

		switch (messageSeverity) {
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
				OC_CORE_TRACE(ss.str());
				break;

			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
				OC_CORE_INFO(ss.str());
				break;

			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
				OC_CORE_WARN(ss.str());
				break;

			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
				OC_CORE_ERROR(ss.str());
				break;
		}

		return VK_FALSE;
	}

	/* ---------- */

	void VulkanInstance::Init() {
		CreateInstance();

		InitDebugMessenger();

		CreateSurface();
	}

	void VulkanInstance::Destroy() {
		vkDestroySurfaceKHR(m_Instance, vkData->Surface, nullptr);

		DestroyDebugUtilsMessengerEXT(m_Instance, m_Messenger, nullptr);

		vkDestroyInstance(m_Instance, nullptr);
	}

	/* HELPER FUNCTIONS */

	static b8 CheckValidationLayerSupport(const std::vector<char*> validationLayers) {
		ui32 layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

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

	static std::vector<const char*> GetRequiredExtensions() {
		ui32 glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (DEBUG_VULKAN)
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		return extensions;
	}

	static void SetDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& info) {
		info = {};

		info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		info.pfnUserCallback = DebugCallback;
	}

	/* ---------------- */

	void VulkanInstance::CreateInstance() {
		if (DEBUG_VULKAN && !CheckValidationLayerSupport(vkData->_ValidationLayers))
			throw std::runtime_error("Validation layers requested, but not available!");

		VkApplicationInfo appInfo{};

		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

		appInfo.pApplicationName = m_AppName.c_str();
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);

		appInfo.pEngineName = "Ocean Engine"; // TODO: Make "Engine Spec"
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

		appInfo.apiVersion = VK_API_VERSION_1_0;

		auto extensions = GetRequiredExtensions();

		VkInstanceCreateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

		info.pApplicationInfo = &appInfo;

		info.enabledExtensionCount = static_cast<ui32>(extensions.size());
		info.ppEnabledExtensionNames = extensions.data();

		if (DEBUG_VULKAN) {
			info.enabledLayerCount = static_cast<ui32>(vkData->_ValidationLayers.size());
			info.ppEnabledLayerNames = vkData->_ValidationLayers.data();

			VkDebugUtilsMessengerCreateInfoEXT messengerinfo{};
			SetDebugMessengerCreateInfo(messengerinfo);

			info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&messengerinfo;
		}
		else {
			info.enabledLayerCount = 0;

			info.pNext = nullptr;
		}

		if (vkCreateInstance(&info, nullptr, &m_Instance) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan instance!");
	}

	void VulkanInstance::InitDebugMessenger() {
		VkDebugUtilsMessengerCreateInfoEXT info{};
		SetDebugMessengerCreateInfo(info);

		if (CreateDebugUtilsMessengerEXT(m_Instance, &info, nullptr, &m_Messenger) != VK_SUCCESS)
			throw std::runtime_error("Failed to set up Vulkan debug messenger!");
	}

	void VulkanInstance::CreateSurface() {
		if (glfwCreateWindowSurface(m_Instance, vkData->Window, nullptr, &vkData->Surface) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan window surface!");
	}

}	// Ocean
