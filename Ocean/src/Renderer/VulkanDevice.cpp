#include "VulkanDevice.hpp"

#include "Renderer/VulkanRenderer.hpp"

#include <GLFW/glfw3.h>

// std
#include <optional>
#include <set>

namespace Ocean {

	namespace Vulkan {

		struct QueueFamilyIndices {
			std::optional<u32> GraphicsFamily;
			std::optional<u32> PresentFamily;

			static constexpr u8 UniqueFamilyCount = 2;

			b8 IsComplete() const {
				return GraphicsFamily.has_value() && PresentFamily.has_value();
			}

		};	// QueueFamilyIndices

		struct SwapChainSupportDetails {
			VkSurfaceCapabilitiesKHR Capabilities;

			std::vector<VkSurfaceFormatKHR> Formats;
			std::vector<VkPresentModeKHR> PresentModes;

		};	// SwapChainSupportDetails


		void Device::Init(DeviceConfig* config) {
			p_Allocator = config->MemAllocator;
			p_Renderer = config->Renderer;

			u32 deviceCount = 0;
			vkEnumeratePhysicalDevices(p_Renderer->GetVulkanInstance(), &deviceCount, nullptr);

			OASSERTM(deviceCount != 0, "Failed to find GPUs with Vulkan support!");

			std::vector<VkPhysicalDevice> devices(deviceCount);
			vkEnumeratePhysicalDevices(p_Renderer->GetVulkanInstance(), &deviceCount, devices.data());

			for (const auto& device : devices) {
				if (IsDeviceSuitable(device)) {
					m_Physical = device;

					break;
				}
			}

			if (m_Physical == VK_NULL_HANDLE)
				OASSERTM(false, "Failed to find a suitable GPU!\n");

			QueueFamilyIndices indices = FindQueueFamilies(m_Physical);
			CreateLogicalDevice(indices);

			vkGetDeviceQueue(m_Device, indices.GraphicsFamily.value(), 0, &m_GraphicsQueue);
			vkGetDeviceQueue(m_Device, indices.PresentFamily.value(), 0, &m_PresentQueue);
		}

		void Device::Shutdown() {
			vkDestroySwapchainKHR(m_Device, m_SwapChain, nullptr);

			vkDestroyDevice(m_Device, nullptr);
		}

		b8 Device::IsDeviceSuitable(VkPhysicalDevice device) {
			QueueFamilyIndices indices = FindQueueFamilies(device);

			b8 extensionsSupported = CheckDeviceExtensionSupport(device);

			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);

			b8 swapChainAdequate = false;
			if (extensionsSupported)
				swapChainAdequate = !swapChainSupport.Formats.empty() && !swapChainSupport.PresentModes.empty();

			/*VkPhysicalDeviceFeatures deviceFeatures;
			vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);*/

			return indices.IsComplete() && extensionsSupported && swapChainAdequate;
		}

		b8 Device::CheckDeviceExtensionSupport(VkPhysicalDevice device) {
			u32 extensionCount;
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

			std::vector<VkExtensionProperties> availableExtensions(extensionCount);
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

			i32  requiredExtensionCount = ArraySize(k_DeviceExtensions);
			for (const auto& extension : availableExtensions) {
				for (u8 i = 0; i < ArraySize(k_DeviceExtensions); i++) {
					if (strcmp(extension.extensionName, k_DeviceExtensions[i]) == 0) {
						requiredExtensionCount--;

						break;
					}
				}

				if (requiredExtensionCount == 0)
					return true;
			}

			return false;
		}

		QueueFamilyIndices Device::FindQueueFamilies(VkPhysicalDevice device) {
			QueueFamilyIndices indices;

			u32 familyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, nullptr);

			std::vector<VkQueueFamilyProperties> families(familyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, families.data());

			for (u32 i = 0; i < familyCount; i++) {
				if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
					indices.GraphicsFamily = i;

				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(device, i, p_Renderer->GetVulkanSurface(), &presentSupport);

				if (presentSupport)
					indices.PresentFamily = i;

				if (indices.IsComplete())
					break;
			}

			return indices;
		}

		void Device::CreateLogicalDevice(QueueFamilyIndices indices) {
			std::vector<VkDeviceQueueCreateInfo> queueInfos;
			std::array<u32, QueueFamilyIndices::UniqueFamilyCount> uniqueQueueFamilies = {
				indices.GraphicsFamily.value(),
				indices.PresentFamily.value()
			};

			queueInfos.reserve(uniqueQueueFamilies.size());
			f32 queuePriority = 1.0f;
			for (u32 queueFamily : uniqueQueueFamilies) {
				VkDeviceQueueCreateInfo queueInfo{ };
				queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

				queueInfo.queueFamilyIndex = queueFamily;

				queueInfo.queueCount = 1;
				queueInfo.pQueuePriorities = &queuePriority;

				queueInfos.push_back(queueInfo);
			}

			VkPhysicalDeviceFeatures deviceFeatures;
			vkGetPhysicalDeviceFeatures(m_Physical, &deviceFeatures);

			VkDeviceCreateInfo createInfo{ };
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

			createInfo.queueCreateInfoCount = queueInfos.size();
			createInfo.pQueueCreateInfos = queueInfos.data();

			createInfo.pEnabledFeatures = &deviceFeatures;

			createInfo.enabledExtensionCount = ArraySize(k_DeviceExtensions);
			createInfo.ppEnabledExtensionNames = k_DeviceExtensions;

			// Newer versions of Vulkan do not require this.
			createInfo.enabledLayerCount = 0;
			createInfo.ppEnabledLayerNames = nullptr;

			CheckResultSuccess(vkCreateDevice(m_Physical, &createInfo, nullptr, &m_Device), "Failed to create logical device!");
		}

		SwapChainSupportDetails Device::QuerySwapChainSupport(VkPhysicalDevice device) {
			SwapChainSupportDetails details;
			VkSurfaceKHR surface = p_Renderer->GetVulkanSurface();

			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.Capabilities);

			u32 formatCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

			if (formatCount != 0) {
				details.Formats.resize(formatCount);

				vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.Formats.data());
			}

			u32 presentModeCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

			if (presentModeCount != 0) {
				details.PresentModes.resize(presentModeCount);

				vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.PresentModes.data());
			}

			return details;
		}

		VkSurfaceFormatKHR Device::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
			for (const auto& format : availableFormats) {
				if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
					return format;
			}

			// An assumption that the first is likely good enough
			return availableFormats[0];
		}

		VkPresentModeKHR Device::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availableModes) {
			for (const auto& mode : availableModes) {
				if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
					return mode;
			}

			// This is the only guaranteed mode available
			return VK_PRESENT_MODE_FIFO_KHR;
		}

		VkExtent2D Device::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
			if (capabilities.currentExtent.width != std::numeric_limits<u32>::max()) {
				return capabilities.currentExtent;
			}
			else {
				i32 width, height;
				glfwGetFramebufferSize((GLFWwindow*)p_WindowHandle, &width, &height);

				VkExtent2D extent = {
					(u32)width, (u32)height
				};

				extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
				extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

				return extent;
			}
		}

		void Device::CreateSwapChain() {
			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(m_Physical);

			VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.Formats);
			VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.PresentModes);
			VkExtent2D extent = ChooseSwapExtent(swapChainSupport.Capabilities);

			u32 imageCount = swapChainSupport.Capabilities.minImageCount + 1;

			// if maxImageCount = 0 then there is no maximum
			if (imageCount > swapChainSupport.Capabilities.maxImageCount && swapChainSupport.Capabilities.maxImageCount != 0)
				imageCount = swapChainSupport.Capabilities.maxImageCount;

			VkSwapchainCreateInfoKHR info{ };
			info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

			info.surface = p_Renderer->GetVulkanSurface();

			info.minImageCount = imageCount;

			info.imageFormat = surfaceFormat.format;
			info.imageColorSpace = surfaceFormat.colorSpace;

			info.imageExtent = extent;

			info.imageArrayLayers = 1;
			info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			QueueFamilyIndices indices = FindQueueFamilies(m_Physical);
			u32 queueFamilyIndices[] = {
				indices.GraphicsFamily.value(),
				indices.PresentFamily.value()
			};

			if (indices.GraphicsFamily.value() != indices.PresentFamily.value()) {
				info.imageSharingMode = VK_SHARING_MODE_CONCURRENT; // TODO: Change to EXCLUSIVE for better performance

				info.queueFamilyIndexCount = 2;
				info.pQueueFamilyIndices = queueFamilyIndices;
			}
			else {
				info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

				info.queueFamilyIndexCount = 0; // Optional
				info.pQueueFamilyIndices = nullptr; // Optional
			}

			info.preTransform = swapChainSupport.Capabilities.currentTransform;

			info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

			info.presentMode = presentMode;
			info.clipped = VK_TRUE;

			info.oldSwapchain = VK_NULL_HANDLE;

			CheckResultSuccess(vkCreateSwapchainKHR(m_Device, &info, nullptr, &m_SwapChain), "Failed to create swap chain!");
		}

	}	// Vulkan

}	// Ocean
