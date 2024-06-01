#pragma once

#include "Core/Types.hpp"

// libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Ocean {

	class VulkanDevice {
	public:
		VulkanDevice() : m_PhysicalDevice(VK_NULL_HANDLE), m_Device(VK_NULL_HANDLE) { }
		~VulkanDevice() = default;

		void Init();
		void Destroy() const;

		VkDevice GetLogical() const { return m_Device; }
		
		VkPhysicalDevice GetPhysical() const { return m_PhysicalDevice; }

	private:
		void PickPhysicalDevice();

		void CreateLogicalDevice();

		/* --- */

		b8 CheckDeviceExtensionSupport(VkPhysicalDevice device);

		b8 IsDeviceSuitable(VkPhysicalDevice device);

		ui8 RateDeviceSuitability(VkPhysicalDevice device);

		/* --- */

		VkPhysicalDevice m_PhysicalDevice;

		VkDevice m_Device;
	};

}
