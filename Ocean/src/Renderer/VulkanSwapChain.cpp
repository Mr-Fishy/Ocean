#include "VulkanSwapChain.hpp"

#include "Renderer/VulkanRenderer.hpp"
#include "Renderer/VulkanDevice.hpp"

#include "Renderer/VulkanInfos.hpp"

#include <GLFW/glfw3.h>

namespace Ocean {

	namespace Vulkan {

		void SwapChain::Init(SwapChainConfig* config) {
			p_Allocator = config->allocator;
			p_Renderer = config->renderer;
			p_Device = config->device;

			m_SwapChain = VK_NULL_HANDLE;

			// Create the window surface
			CHECK_RESULT(
				glfwCreateWindowSurface(p_Renderer->GetVulkanInstance(), (GLFWwindow*)config->window, nullptr, &m_Surface),
				"Failed to create window surface!"
			);

			VkPhysicalDevice physicalDevice = p_Device->GetPhysical();

			// Choose Family Queue Nodes
			u32 queueCount;
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, nullptr);

			FixedArray<VkQueueFamilyProperties> queueFamilies(queueCount);
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, queueFamilies.Data());
			queueFamilies.SetSize(queueCount);

			// Find families that don't support present
			FixedArray<VkBool32> supportsPresentation(queueCount);
			for (u32 i = 0; i < queueCount; i++)
				vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, m_Surface, &supportsPresentation.Get(i));
			supportsPresentation.SetSize(queueCount);

			u32 graphicsIndex = u32_max;
			u32 presentIndex = u32_max;
			for (u32 i = 0; i < queueCount; i++) {
				if (queueFamilies.Get(i).queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					if (graphicsIndex == u32_max)
						graphicsIndex = i;

					if (supportsPresentation.Get(i) == VK_TRUE) { // Best case a queue support both
						graphicsIndex = presentIndex = i;

						break;
					}
				}
			}

			// Only if no queue supports both
			if (presentIndex == u32_max) {
				for (u32 i = 0; i < queueCount; i++) {
					if (supportsPresentation.Get(i) == VK_TRUE) {
						presentIndex = i;

						break;
					}
				}
			}

			OASSERTM(graphicsIndex != u32_max, "Could not find a graphics queue family!");
			OASSERTM(presentIndex != u32_max, "Could not find a presentation queue family!");

			m_QueueNodeIndices.GraphicsIndex = graphicsIndex;
			m_QueueNodeIndices.PresentationIndex = presentIndex;

			// Get SwapChain Format & Color Space
			u32 formatCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_Surface, &formatCount, nullptr);

			FixedArray<VkSurfaceFormatKHR> formats(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_Surface, &formatCount, formats.Data());
			formats.SetSize(formatCount);

			VkFormat preferredFormats[] = {
				VK_FORMAT_R8G8B8A8_UNORM,
				VK_FORMAT_B8G8R8A8_UNORM,
				VK_FORMAT_A8B8G8R8_UNORM_PACK32
			};
			u32 selectedFormatIndex = 0;
			for (u8 i = 0; i < ArraySize(preferredFormats); i++) {
				for (u32 k = 0; k < formats.Size(); k++) {
					if (formats.Get(k).format == preferredFormats[i]) {
						selectedFormatIndex = k;
						break;
					}
				}
			}

			m_ColorFormat = formats.Get(selectedFormatIndex).format;
			m_ColorSpace = formats.Get(selectedFormatIndex).colorSpace;
		}

		void SwapChain::Shutdown() {
			if (m_SwapChain != VK_NULL_HANDLE) {
				for (u32 i = 0; i < m_ImageCount; i++)
					vkDestroyImageView(p_Device->GetLogical(), m_Buffers.Get(i).View, nullptr);
			}

			if (m_Surface != VK_NULL_HANDLE) {
				vkDestroySwapchainKHR(p_Device->GetLogical(), m_SwapChain, nullptr);

				vkDestroySurfaceKHR(p_Renderer->GetVulkanInstance(), m_Surface, nullptr);
			}

			m_Surface = VK_NULL_HANDLE;
			m_SwapChain = VK_NULL_HANDLE;
		}



		void SwapChain::CreateSwapChain(u32* width, u32* height, b8 vsync, b8 fullscreen) {
			VkSwapchainKHR oldSwapChain = m_SwapChain;

			VkPhysicalDevice physicalDevice = p_Device->GetPhysical();
			VkDevice device = p_Device->GetLogical();

			VkSurfaceCapabilitiesKHR surfaceCaps;
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, m_Surface, &surfaceCaps);

			u32 presentModeCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_Surface, &presentModeCount, nullptr);

			FixedArray<VkPresentModeKHR> presentModes(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_Surface, &presentModeCount, presentModes.Data());
			presentModes.SetSize(presentModeCount);

			VkExtent2D swapChainExtent{ };
			// If width (and height) is the special value 0xFFFFFFFF, the size of the surface will be set by the swapchain
			if (surfaceCaps.currentExtent.width == 0xFFFFFFFF) {
				swapChainExtent.width = *width;
				swapChainExtent.height = *height;
			}
			else {
				swapChainExtent = surfaceCaps.currentExtent;

				*width = surfaceCaps.currentExtent.width;
				*height = surfaceCaps.currentExtent.height;
			}

			// This is the default mode and waits for the vertical blank ("v-sync")
			VkPresentModeKHR swapChainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

			if (!vsync) {
				for (u32 i = 0; i < presentModeCount; i++) {
					if (presentModes.Get(i) == VK_PRESENT_MODE_MAILBOX_KHR) {
						swapChainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
						break;
					}
					else if (presentModes.Get(i) == VK_PRESENT_MODE_IMMEDIATE_KHR) {
						swapChainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
					}
				}
			}

			u32 desiredSwapChainImages = surfaceCaps.minImageCount + 1;
			if (surfaceCaps.maxImageCount > 0 && desiredSwapChainImages > surfaceCaps.maxImageCount)
				desiredSwapChainImages = surfaceCaps.maxImageCount;

			VkSurfaceTransformFlagsKHR preTransform;
			if (surfaceCaps.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
				preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
			else
				preTransform = surfaceCaps.currentTransform;

			VkCompositeAlphaFlagBitsKHR alphaFlagBits[] = {
				VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
				VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
				VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
				VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR
			};
			VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			for (u8 i = 0; i < ArraySize(alphaFlagBits); i++) {
				if (surfaceCaps.supportedCompositeAlpha & compositeAlpha) {
					compositeAlpha = alphaFlagBits[i];
					break;
				}
			}

			VkSwapchainCreateInfoKHR info{ };
			info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

			info.surface = m_Surface;

			info.minImageCount = desiredSwapChainImages;

			info.imageFormat = m_ColorFormat;
			info.imageColorSpace = m_ColorSpace;
			info.imageExtent = { swapChainExtent.width, swapChainExtent.height };
			info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			info.preTransform = (VkSurfaceTransformFlagBitsKHR)preTransform;

			info.imageArrayLayers = 1;
			info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

			info.queueFamilyIndexCount = 0;

			info.presentMode = swapChainPresentMode;

			info.oldSwapchain = oldSwapChain;

			info.clipped = VK_TRUE;
			info.compositeAlpha = compositeAlpha;

			if (surfaceCaps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
				info.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

			if (surfaceCaps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
				info.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;

			CHECK_RESULT(
				vkCreateSwapchainKHR(device, &info, nullptr, &m_SwapChain),
				"Failed to create swapchain!"
			);

			// Destroy old swapchain if it exists
			if (oldSwapChain != VK_NULL_HANDLE) {
				for (u32 i = 0; i < m_ImageCount; i++)
					vkDestroyImageView(device, m_Buffers.Get(i).View, nullptr);

				vkDestroySwapchainKHR(device, oldSwapChain, nullptr);
			}

			CHECK_RESULT(
				vkGetSwapchainImagesKHR(device, m_SwapChain, &m_ImageCount, nullptr),
				"Failed to get swapchain images!"
			);

			m_Images.Clear();
			m_Images.Resize(m_ImageCount);
			CHECK_RESULT(
				vkGetSwapchainImagesKHR(device, m_SwapChain, &m_ImageCount, m_Images.Data()),
				"Failed to get swapchain images!"
			);

			m_Buffers.Clear();
			m_Buffers.Resize(m_ImageCount);
			for (u32 i = 0; i < m_ImageCount; i++) {
				VkImageViewCreateInfo imageInfo = ColorAttachmentCreateInfo(m_ColorFormat, m_Buffers.Get(i).Image = m_Images.Get(i));

				CHECK_RESULT(
					vkCreateImageView(device, &imageInfo, nullptr, &m_Buffers.Get(i).View),
					"Failed to create image view!"
				);
			}
		}

		VkResult SwapChain::GetNextImage(VkSemaphore presentComplete, u32* imageIndex) {
			// By setting the timeout time to u64_max it will always wait until the next image is ready.
			return vkAcquireNextImageKHR(p_Device->GetLogical(), m_SwapChain, u64_max, presentComplete, nullptr, imageIndex);
		}

		VkResult SwapChain::QueuePresentation(VkQueue queue, u32 imageIndex, VkSemaphore waitSemaphore) {
			VkPresentInfoKHR info{ };
			info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

			info.pNext = nullptr;

			info.swapchainCount = 1;
			info.pSwapchains = &m_SwapChain;

			info.pImageIndices = &imageIndex;

			if (waitSemaphore != VK_NULL_HANDLE) {
				info.waitSemaphoreCount = 1;
				info.pWaitSemaphores = &waitSemaphore;
			}

			return vkQueuePresentKHR(queue, &info);
		}

	}	// Vulkan

}	// Ocean
