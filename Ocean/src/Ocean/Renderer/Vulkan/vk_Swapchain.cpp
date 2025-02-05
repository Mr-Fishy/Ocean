#include "vk_Swapchain.hpp"

#include "Ocean/Primitives/Numerics.hpp"
#include "Ocean/Types/FloatingPoints.hpp"

#include "Ocean/Primitives/Memory.hpp"
#include "Ocean/Primitives/Exceptions.hpp"

#include "Ocean/Renderer/Vulkan/vk_Vulkan.hpp"
#include "Ocean/Renderer/Vulkan/vk_Instance.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {

        vkSwapchain::vkSwapchain(VkSurfaceKHR surface) :
            m_Surface(surface),
            m_Swapchain(VK_NULL_HANDLE),
            m_Queue(VK_NULL_HANDLE),
            m_GraphicsQueueIndex(0),
            m_PresentQueueIndex(0),
            m_Format(),
            m_ColorSpace(),
            m_Extent(),
            m_Images(0)
        {
            u32 queueCount;
            vkGetPhysicalDeviceQueueFamilyProperties(vkInstance::Get().Device()->GetPhysical(), &queueCount, nullptr);

            VkQueueFamilyProperties* queueProperties = oallocat(VkQueueFamilyProperties, queueCount, oSystemAllocator);
            vkGetPhysicalDeviceQueueFamilyProperties(vkInstance::Get().Device()->GetPhysical(), &queueCount, queueProperties);

            b32* supportsPresentation = oallocat(b32, queueCount, oSystemAllocator);
            for (u32 i = 0; i < queueCount; i++)
                vkGetPhysicalDeviceSurfaceSupportKHR(vkInstance::Get().Device()->GetPhysical(), i, surface, &supportsPresentation[i]);

            // Search for a graphics and presentation queue in the array of queue families,
            // ideally find one that supports both.
            //
            for (u32 i = 0; i < queueCount; i++) {
                if (queueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    if (supportsPresentation[i] == VK_TRUE) {
                        this->m_GraphicsQueueIndex = i;
                        this->m_PresentQueueIndex = i;

                        break;
                    }
                    else if (this->m_GraphicsQueueIndex == u32_max) {
                        this->m_GraphicsQueueIndex = i;
                    }
                }
            }

            // If a queue that supports both was not found, then find a separate presentation queue.
            //
            if (this->m_PresentQueueIndex == u32_max) {
                for (u32 i = 0; i < queueCount; i++) {
                    if (supportsPresentation[i] == VK_TRUE) {
                        this->m_PresentQueueIndex = i;

                        break;
                    }
                }
            }

            ofree(supportsPresentation, oSystemAllocator);
            ofree(queueProperties, oSystemAllocator);

            if (this->m_GraphicsQueueIndex == u32_max || this->m_PresentQueueIndex == u32_max)
                throw Exception(Error::SYSTEM_ERROR, "Could not find a graphics and presentation queue.");

            /** @todo Add support for separate queues, including presentation, synchronization, and QueueSubmit tracking. */

            f32 queuePriorities[] = {
                0.0f
            };

            VkDeviceQueueCreateInfo queueInfo = {
                VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                nullptr,
                0,
                this->m_GraphicsQueueIndex,
                1,
                queuePriorities
            };

            vkInstance::Get().Device()->InitLogicalDevice(queueInfo);

            vkGetDeviceQueue(vkInstance::Get().Device()->GetLogical(), this->m_GraphicsQueueIndex, 0, &this->m_Queue);

            // Get the list of formats that are supported by the surface.
            //
            u32 formatCount;
            vkCheck(
                vkGetPhysicalDeviceSurfaceFormatsKHR(vkInstance::Get().Device()->GetPhysical(), surface, &formatCount, nullptr)
            );

            VkSurfaceFormatKHR* surfaceFormats = oallocat(VkSurfaceFormatKHR, formatCount, oSystemAllocator);
            vkCheck(
                vkGetPhysicalDeviceSurfaceFormatsKHR(vkInstance::Get().Device()->GetPhysical(), surface, &formatCount, surfaceFormats)
            );

            // If the supported formats include just one entry of VK_FORMAT_UNDEFINED, the surface has no preferred format.
            // Otherwise, at least one supported format will be returned.
            //
            if (formatCount == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED) {
                this->m_Format = VK_FORMAT_B8G8R8A8_UNORM;
            }
            else if (formatCount >= 1) {
                /** @todo Make formats determined based on a preference. Same for the colorspace. */

                this->m_Format = surfaceFormats[0].format;
            }

            this->m_ColorSpace = surfaceFormats[0].colorSpace;

            ofree(surfaceFormats, oSystemAllocator);

            CreateSwapchain();
        }

        vkSwapchain::~vkSwapchain() {
            
        }

        void vkSwapchain::CreateSwapchain() {
            // Save the old swapchain to be used for faster recreation.
            //
            VkSwapchainKHR oldSwapchain = this->m_Swapchain;

            // ================================ GET CURRENT SURFACE INFO ================================
            //
            // Get the surface capabilites to ensure that the new swapchain is valid for the surface.
            //
            VkSurfaceCapabilitiesKHR surfaceCapabilities;
            vkCheck(
                vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkInstance::Get().Device()->GetPhysical(), this->m_Surface, &surfaceCapabilities)
            );

            // If the surface size is undefined, the size is set to the size of the images requested,
            // which must fit within the minimum and maximum values.
            //
            if (surfaceCapabilities.currentExtent.width == 0xFFFFFFFF) {
                /** @todo Get the prefered width and height for the swapchain. */

                this->m_Extent.width = oClamp(this->m_Extent.width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
                this->m_Extent.height = oClamp(this->m_Extent.height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);
            }
            else {
                this->m_Extent = surfaceCapabilities.currentExtent;

                /** @todo Propagate dimension update if necessary. */
            }

            VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;

            // Determine the numb of VkImage's to use in the swapchain.
            // The application desires only 1 image at a time, represented by "minImageCount".
            //
            u32 desiredSwapchainImageCount = surfaceCapabilities.minImageCount;

            // If the "maxImageCount" is 0, then we can use as many images as we want.
            // Otherwise we're limited to "maxImageCount".
            //
            if (surfaceCapabilities.maxImageCount > 0 && desiredSwapchainImageCount > surfaceCapabilities.maxImageCount)
                desiredSwapchainImageCount = surfaceCapabilities.maxImageCount;

            VkSurfaceTransformFlagBitsKHR preTransform;
            if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
                preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
            else
                preTransform = surfaceCapabilities.currentTransform;


            // ================================ NEW SWAPCHAIN ================================
            //
            const VkSwapchainCreateInfoKHR swapchainInfo {
                VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
                nullptr,
                0,
                this->m_Surface,
                desiredSwapchainImageCount,
                this->m_Format,
                this->m_ColorSpace,
                this->m_Extent,
                1,
                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                VK_SHARING_MODE_EXCLUSIVE,
                0,
                nullptr,
                preTransform,
                VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
                presentMode,
                true,
                oldSwapchain
            };

            vkCheck(
                vkCreateSwapchainKHR(vkInstance::Get().Device()->GetLogical(), &swapchainInfo, nullptr, &this->m_Swapchain)
            );

            // ================================ CLEANUP OLD SWAPCHAIN ================================
            //
            // If the swapchain was just re-created, then the old one needs to be destroyed.
            //
            // Note: Destroying the swapchain also cleans up all of its associated images once they are out of use.
            //
            if (oldSwapchain != VK_NULL_HANDLE)
                vkDestroySwapchainKHR(vkInstance::Get().Device()->GetLogical(), oldSwapchain, nullptr);

            // ================================ RECREATE SWAPCHAIN IMAGES ================================
            //
            // Now we can recreate the swapchain images.
            //
            u32 swapchainImageCount;
            vkCheck(
                vkGetSwapchainImagesKHR(vkInstance::Get().Device()->GetLogical(), this->m_Swapchain, &swapchainImageCount, nullptr)
            );

            VkImage* swapchainImages = oallocat(VkImage, swapchainImageCount, oSystemAllocator);
            vkCheck(
                vkGetSwapchainImagesKHR(vkInstance::Get().Device()->GetLogical(), this->m_Swapchain, &swapchainImageCount, swapchainImages)
            );

            // Reserve so that if there is more images than the last it will grow ahead of time.
            //
            this->m_Images.reserve(swapchainImageCount);

            // Create all of the image views for each image.
            //
            for (u32 i = 0; i < swapchainImageCount; i++) {
                VkImageViewCreateInfo colorViewInfo {
                    VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                    nullptr,
                    0,
                    (this->m_Images[i].image = swapchainImages[i]),
                    VK_IMAGE_VIEW_TYPE_2D,
                    this->m_Format,
                    {
                        VK_COMPONENT_SWIZZLE_R,
                        VK_COMPONENT_SWIZZLE_G,
                        VK_COMPONENT_SWIZZLE_B,
                        VK_COMPONENT_SWIZZLE_A
                    },
                    {
                        VK_IMAGE_ASPECT_COLOR_BIT,
                        0,
                        1,
                        0,
                        1
                    }
                };

                vkCheck(
                    vkCreateImageView(vkInstance::Get().Device()->GetLogical(), &colorViewInfo, nullptr, &this->m_Images[i].view)
                );
            }

            ofree(swapchainImages, oSystemAllocator);
        }

    } // namespace Splash

} // namespace Ocean
