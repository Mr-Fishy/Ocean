#include "ocpch.hpp"

#include "Vk/VulkanDevice.hpp"

#include "Vk/VulkanData.hpp"
#include "Ocean/Logger/Log.hpp"

// std
#include <map>
#include <set>

namespace Ocean {

	void VulkanDevice::Init() {
		PickPhysicalDevice();

		CreateLogicalDevice();
	}

	void VulkanDevice::Destroy() const {
		vkDestroyDevice(m_Device, nullptr);
	}

	void VulkanDevice::PickPhysicalDevice() {
		ui32 deviceCount = 0;
		vkEnumeratePhysicalDevices(vkData->Instance.GetInstance(), &deviceCount, nullptr);

		if (deviceCount == 0)
			throw std::runtime_error("Failed to find GPUs with Vulkan support!");

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(vkData->Instance.GetInstance(), &deviceCount, devices.data());

		std::multimap<ui8, VkPhysicalDevice> candidates;

		for (const auto& device : devices) {
			candidates.insert(std::make_pair(RateDeviceSuitability(device), device));
		}

		if (candidates.rbegin()->first > 0)
			m_PhysicalDevice = candidates.rbegin()->second;
		else
			throw std::runtime_error("Failed to find a suitable GPU!");
	}

	void VulkanDevice::CreateLogicalDevice() {
		QueueFamilyIndices indices = QueueFamilyIndices::FindQueueFamilies(m_PhysicalDevice, vkData->Surface);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<ui32> uniqueQueueFamilies = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

		f32 queuePriority = 1.0f;
		for (ui32 queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};

			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.pQueuePriorities = &queuePriority;

			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};

		deviceFeatures.samplerAnisotropy = VK_TRUE;

		VkDeviceCreateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		info.queueCreateInfoCount = static_cast<ui32>(queueCreateInfos.size());
		info.pQueueCreateInfos = queueCreateInfos.data();

		info.pEnabledFeatures = &deviceFeatures;

		info.enabledExtensionCount = static_cast<ui32>(vkData->_DeviceExtensions.size());
		info.ppEnabledExtensionNames = vkData->_DeviceExtensions.data();
		
		if (DEBUG_VULKAN) {
			info.enabledLayerCount = static_cast<ui32>(vkData->_ValidationLayers.size());
			info.ppEnabledLayerNames = vkData->_ValidationLayers.data();
		}
		else {
			info.enabledLayerCount = 0;
		}

		if (vkCreateDevice(m_PhysicalDevice, &info, nullptr, &m_Device))
			throw std::runtime_error("Failed to create logical device!");

		vkGetDeviceQueue(m_Device, indices.GraphicsFamily.value(), 0, &vkData->GraphicsQueue);
		vkGetDeviceQueue(m_Device, indices.PresentFamily.value(), 0, &vkData->PresentQueue);
	}

	/* HELPER FUNCTIONS */

	b8 VulkanDevice::CheckDeviceExtensionSupport(VkPhysicalDevice device) {
		ui32 extensionCount = 0;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(vkData->_DeviceExtensions.begin(), vkData->_DeviceExtensions.end());

		for (const auto& extension : availableExtensions)
			requiredExtensions.erase(extension.extensionName);

		return requiredExtensions.empty();
	}

	b8 VulkanDevice::IsDeviceSuitable(VkPhysicalDevice device) {
		QueueFamilyIndices indices = QueueFamilyIndices::FindQueueFamilies(device, vkData->Surface);

		bool extensionsSupported = CheckDeviceExtensionSupport(device);

		bool swapChainAdequate = false;
		if (extensionsSupported) {
			SwapchainSupportDetails swapChainSupport = SwapchainSupportDetails::QuerySwapchainSupport(device, vkData->Surface);
			swapChainAdequate = !swapChainSupport.Formats.empty() && !swapChainSupport.PresentModes.empty();
		}

		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

		return indices.IsComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
	}

	ui8 VulkanDevice::RateDeviceSuitability(VkPhysicalDevice device) {
		ui8 score = 0;

		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(device, &properties);

		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(device, &features);

		if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			score += 10;

		score += properties.limits.maxImageDimension2D % 100;

		if (!features.geometryShader || !IsDeviceSuitable(device))
			return 0;

		std::stringstream ss;

		ss << "Vulkan |: Phsyical Device : " << properties.deviceName;

		OC_CORE_INFO(ss.str());

		return score;
	}

	/* ---------------- */

}	// Ocean
