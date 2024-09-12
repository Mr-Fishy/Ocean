#include "VulkanDevice.hpp"

#include "Renderer/VulkanRenderer.hpp"
#include "Renderer/VulkanInfos.hpp"

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

			CreateCommandPool(indices);
		}

		void Device::Shutdown() {
			vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);

			vkDestroyDevice(m_Device, nullptr);

			m_VertShader.Shutdown();
			m_FragShader.Shutdown();
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

			b8 canBeShared = false;
			for (u32 i = 0; i < familyCount; i++) {
				if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
					indices.GraphicsFamily = i;

				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(device, i, p_Renderer->GetVulkanSurface(), &presentSupport);

				if (presentSupport) {
					if (indices.GraphicsFamily.value() == i)
						canBeShared = true;
					else
						indices.PresentFamily = i;
				}

				if (indices.IsComplete())
					break;
			}

			if (!indices.PresentFamily.has_value() && canBeShared)
				indices.PresentFamily.value() = indices.GraphicsFamily.value();

			return indices;
		}

		void Device::CreateLogicalDevice(QueueFamilyIndices indices) {
			std::vector<VkDeviceQueueCreateInfo> queueInfos;
			std::array<u32, QueueFamilyIndices::UniqueFamilyCount> uniqueQueueFamilies = {
				indices.GraphicsFamily.value(),
				indices.PresentFamily.value()
			};

			f32 queuePriority = 1.0f;
			for (u32 queueFamily : uniqueQueueFamilies) {
				VkDeviceQueueCreateInfo queueInfo{ };
				queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

				queueInfo.queueFamilyIndex = queueFamily;

				queueInfo.queueCount = 1;
				queueInfo.pQueuePriorities = &queuePriority;

				queueInfos.emplace_back(queueInfo);
			}

			VkPhysicalDeviceFeatures deviceFeatures{ };
			vkGetPhysicalDeviceFeatures(m_Physical, &deviceFeatures);

			VkDeviceCreateInfo createInfo{ };
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

			createInfo.queueCreateInfoCount = (u32)queueInfos.size();
			createInfo.pQueueCreateInfos = queueInfos.data();

			createInfo.pEnabledFeatures = &deviceFeatures;

			createInfo.enabledExtensionCount = ArraySize(k_DeviceExtensions);
			createInfo.ppEnabledExtensionNames = k_DeviceExtensions;

			// Newer versions of Vulkan do not require this.
			createInfo.enabledLayerCount = 0;
			createInfo.ppEnabledLayerNames = nullptr;

			CHECK_RESULT(
				vkCreateDevice(m_Physical, &createInfo, nullptr, &m_Device),
				"Failed to create logical device!"
			);
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

		

		void Device::CreateCommandPool(QueueFamilyIndices indices) {
			VkCommandPoolCreateInfo info{ };
			info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

			info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

			info.queueFamilyIndex = indices.GraphicsFamily.value();

			CHECK_RESULT(
				vkCreateCommandPool(m_Device, &info, nullptr, &m_CommandPool),
				"Failed to create command pool!"
			);
		}

		VkCommandBuffer Device::CreateCommandBuffer() {
			VkCommandBufferAllocateInfo allocInfo = CommandBufferAllocateInfo(
				m_CommandPool,
				VK_COMMAND_BUFFER_LEVEL_PRIMARY,
				0
			);

			CHECK_RESULT(
				vkAllocateCommandBuffers(m_Device, &allocInfo, &m_CommandBuffer),
				"Failed to allocate command buffers!"
			);

			return m_CommandBuffer;
		}

		void Device::RecordCommandBuffer(VkCommandBuffer, u32 imageIndex) {
			BeginCommandBuffer(m_CommandBuffer);

			{
				BeginRenderPass(RenderPassInfo(m_CommandBuffer, m_RenderPass, m_Framebuffers.Get(imageIndex).GetFrame(), m_SwapChainExtent));

				{
					vkCmdBindPipeline(m_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);

					VkViewport viewport{ };
					viewport.x = 0.0f;
					viewport.y = 0.0f;

					viewport.width = (f32)m_SwapChainExtent.width;
					viewport.height = (f32)m_SwapChainExtent.height;

					viewport.minDepth = 0.0f;
					viewport.maxDepth = 1.0f;

					vkCmdSetViewport(m_CommandBuffer, 0, 1, &viewport);

					VkRect2D scissor{ };
					scissor.offset = { 0, 0 };
					scissor.extent = m_SwapChainExtent;

					vkCmdSetScissor(m_CommandBuffer, 0, 1, &scissor);

					vkCmdDraw(m_CommandBuffer, 3, 1, 0, 0);
				}

				EndRenderPass(m_CommandBuffer);
			}

			EndCommandBuffer(m_CommandBuffer);
		}

	}	// Vulkan

}	// Ocean
