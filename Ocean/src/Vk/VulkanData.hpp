#pragma once

#include "Core/Types.hpp"

#include "Vk/VulkanInstance.hpp"

#include "Vk/VulkanDevice.hpp"

#include "Vk/VulkanSwapchain.hpp"

#include "Vk/VulkanRenderer.hpp"
#include "Vk/VulkanDescriptorSet.hpp"

#include "Vk/VulkanFramebuffer.hpp"

#include "Vk/VulkanBuffer.hpp"

#include "Vk/VulkanTexture.hpp"
#include "Vk/VulkanSampler.hpp"

#include "Vk/VulkanVIBuffer.hpp"
#include "Vk/VulkanUniformBuffer.hpp"

#include "Vk/VulkanSync.hpp"

// libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// std
#include <optional>

namespace Ocean {

	#ifdef OC_DEBUG
	constexpr auto DEBUG_VULKAN = true;
	#else
	constexpr auto DEBUG_VULKAN = false;
	#endif // OC_DEBUG

	struct ContextSpecs {
		std::string AppName;

		ui8 MaxFramesInFlight;

		GLFWwindow* Window;

		ContextSpecs(const std::string& name, ui8 frames, GLFWwindow* window) : AppName(name), MaxFramesInFlight(frames), Window(window) { }
	};

	struct VulkanData {
		VulkanInstance Instance;
		VulkanDevice Device;

		VulkanSwapchain Swapchain;

		VulkanDescriptorSet Descriptor;
		VulkanRenderer Renderer;

		VulkanFramebuffer Framebuffer;

		VulkanBuffer Buffer;

		VulkanTexture* StaticTexture;
		VulkanDepthTexture DepthTexture;
		std::vector<VulkanTexture> Textures;

		VulkanSampler Sampler;

		VulkanVertexBuffer VertexBuffer;
		VulkanIndexBuffer IndexBuffer;
		VulkanUniformBuffer UniformBuffer;

		VulkanSync Sync;

		/* --- */

		GLFWwindow* Window;

		VkSurfaceKHR Surface;

		VkQueue GraphicsQueue;
		VkQueue PresentQueue;

		ui8 MaxFramesInFlight;

		ui8 CurrentFrame = 0;

		const std::vector<char*> _ValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		const std::vector<char*> _DeviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		VulkanData(ContextSpecs spec);
		~VulkanData();

		void Init();
		void Destroy();

	private:
		VulkanData(const VulkanData& src);
		VulkanData(VulkanData&& src);
	};

	extern VulkanData* vkData;

	/* --- */

	struct QueueFamilyIndices {
		std::optional<ui32> GraphicsFamily;
		std::optional<ui32> PresentFamily;

		bool IsComplete() const {
			return GraphicsFamily.has_value() && PresentFamily.has_value();
		}

		static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
			QueueFamilyIndices indices;

			ui32 queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

			i32 i = 0;
			for (const auto& queueFamily : queueFamilies) {
				if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
					indices.GraphicsFamily = i;

				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
				if (presentSupport)
					indices.PresentFamily = i;

				if (indices.IsComplete())
					break;

				i++;
			}

			return indices;
		}
	};

	struct SwapchainSupportDetails {
		VkSurfaceCapabilitiesKHR Capabilities{};

		std::vector<VkSurfaceFormatKHR> Formats;
		std::vector<VkPresentModeKHR> PresentModes;

		static SwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
			SwapchainSupportDetails details;

			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.Capabilities);

			ui32 formatCount = 0;
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
			if (formatCount != 0) {
				details.Formats.resize(formatCount);
				vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.Formats.data());
			}

			ui32 presentModeCount = 0;
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
			if (presentModeCount != 0) {
				details.PresentModes.resize(presentModeCount);
				vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.PresentModes.data());
			}

			return details;
		}
	};

	struct Vertex {
		vec3 pos;
		vec3 color;
		vec2 texCoord;

		static VkVertexInputBindingDescription GetBindingDescription() {
			VkVertexInputBindingDescription desc{};

			desc.binding = 0;
			desc.stride = sizeof(Vertex);
			desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return desc;
		}

		static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, color);

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

			return attributeDescriptions;
		}
	};

	struct UniformBufferObject {
		mat4 model;
		mat4 view;
		mat4 proj;
	};

	namespace VkUtils {

		inline VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) {
			VkImageViewCreateInfo info{};

			info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

			info.image = image;
			info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			info.format = format;

			info.subresourceRange.aspectMask = aspectFlags;
			info.subresourceRange.baseMipLevel = 0;
			info.subresourceRange.levelCount = 1;
			info.subresourceRange.baseArrayLayer = 0;
			info.subresourceRange.layerCount = 1;

			VkImageView imageView;
			if (vkCreateImageView(vkData->Device.GetLogical(), &info, nullptr, &imageView) != VK_SUCCESS)
				throw std::runtime_error("Failed to create Vulkan image view!");

			return imageView;
		}

		inline ui32 FindMemoryType(ui32 typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice device) {
			VkPhysicalDeviceMemoryProperties mem;
			vkGetPhysicalDeviceMemoryProperties(device, &mem);

			for (ui32 i = 0; i < mem.memoryTypeCount; i++) {
				if (typeFilter & (1 << i) && (mem.memoryTypes[i].propertyFlags & properties) == properties)
					return i;
			}

			throw std::runtime_error("Failed to find suitable Vulkan memory type!");
		}

	}	// VkUtils

}
