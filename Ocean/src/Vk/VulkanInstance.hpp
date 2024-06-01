#pragma once

#include "Core/Types.hpp"

// libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Ocean {

	class VulkanInstance {
	public:
		VulkanInstance(const std::string& appName) : m_Instance(VK_NULL_HANDLE), m_Messenger(VK_NULL_HANDLE), m_AppName(appName) { }
		~VulkanInstance() = default;

		void Init();
		void Destroy();

		VkInstance GetInstance() const { return m_Instance; }

	private:
		void CreateInstance();

		void InitDebugMessenger();

		void CreateSurface();

		/* --- */

		VkInstance m_Instance;

		VkDebugUtilsMessengerEXT m_Messenger;

		// TODO: Make char* within "App Spec"
		std::string m_AppName;
	};

}
