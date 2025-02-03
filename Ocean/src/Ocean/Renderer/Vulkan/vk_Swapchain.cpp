#include "vk_Swapchain.hpp"

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
            m_Swapchain(VK_NULL_HANDLE),
            m_Format(),
            m_Extent(),
            m_Images(0),
            m_IsValid(false)
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
        }

        vkSwapchain::~vkSwapchain() {
            
        }

    } // namespace Splash

} // namespace Ocean
