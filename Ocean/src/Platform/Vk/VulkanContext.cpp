#include "ocpch.hpp"

#include "Platform/Vk/VulkanContext.hpp"

#include "Ocean/Core/Log.hpp"

namespace Ocean::Vk {

	VulkanContext::VulkanContext(GLFWwindow* window) : m_Window(window), m_Instance(VK_NULL_HANDLE) {
		if (!m_Window)
			throw std::runtime_error("Window handle is null!");
	}

	void VulkanContext::Init() {
		CreateInstance();
	}

	void VulkanContext::SwapBuffers() {

	}

	void VulkanContext::CreateInstance() {
		VkApplicationInfo appInfo{};

		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

		appInfo.pApplicationName = "Ocean Application";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);

		appInfo.pEngineName = "Ocean Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

		appInfo.apiVersion = VK_API_VERSION_1_3;

		ui32 glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		VkInstanceCreateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

		info.pApplicationInfo = &appInfo;

		info.enabledExtensionCount = glfwExtensionCount;
		info.ppEnabledExtensionNames = glfwExtensions;

		info.enabledLayerCount = 0;
		info.ppEnabledLayerNames = nullptr;

		if (vkCreateInstance(&info, nullptr, &m_Instance) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan instance!");



	}

}	// Ocean::Vk
