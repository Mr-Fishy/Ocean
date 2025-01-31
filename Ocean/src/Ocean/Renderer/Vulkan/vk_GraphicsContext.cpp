#include "vk_GraphicsContext.hpp"

#include "Ocean/Primitives/Exceptions.hpp"
#include "Ocean/Types/Integers.hpp"

#include "Ocean/Primitives/Memory.hpp"
#include "Ocean/Primitives/Array.hpp"
#include "Ocean/Primitives/Assert.hpp"

#include "Ocean/Renderer/RenderCommand.hpp"

#include "Ocean/Renderer/Vulkan/vk_RendererAPI.hpp"

// libs
#define GLFW_INCLUDE_NONE
#include <glad/vulkan.h>
#include <GLFW/glfw3.h>

namespace Ocean {

    namespace Splash {
    
        vkGraphicsContext::vkGraphicsContext(GLFWwindow* window) : p_WindowHandle(window), m_Surface(VK_NULL_HANDLE), m_Swapchain(VK_NULL_HANDLE), m_SwapchainImages(), m_Format(), m_Extent() {
            OASSERTM(this->p_WindowHandle != nullptr, "Window Handle Is A nullptr!");
        }

        vkGraphicsContext::~vkGraphicsContext() {

        }

        void vkGraphicsContext::Init() {
            vkRendererAPI* api = static_cast<vkRendererAPI*>(RenderCommand::GetRenderer().get());

            glfwCreateWindowSurface(api->GetInstance(), this->p_WindowHandle, nullptr, &this->m_Surface);

            DynamicArray<VkQueueFamilyProperties>& queueProperties = api->GetQueueProperties();

            b32* supportsPresentation = oallocat(b32, queueProperties.size(), oSystemAllocator);
            for (u32 i = 0; i < queueProperties.size(); i++)
                vkGetPhysicalDeviceSurfaceSupportKHR(api->GetPhysicalDevice(), i, this->m_Surface, &supportsPresentation[i]);

            u32 graphicsQueueIndex = u32_max;
            u32 presentQueueIndex = u32_max;
            for (u32 i = 0; i < queueProperties.size(); i++) {
                if ((queueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
                    graphicsQueueIndex = i;

                if (supportsPresentation[i] == VK_TRUE) {
                    graphicsQueueIndex = presentQueueIndex = i;

                    break;
                }
            }

            if (presentQueueIndex == u32_max) {
                for (u32 i = 0; i < queueProperties.size(); i++) {
                    if (supportsPresentation[i] == VK_TRUE) {
                        presentQueueIndex = i;

                        break;
                    }
                }
            }

            ofree(supportsPresentation, oSystemAllocator);

            if (graphicsQueueIndex == u32_max || presentQueueIndex == u32_max)
                throw Exception(Error::SYSTEM_ERROR, "Could not find a graphics and presentation queue! GraphicsContext Init Failure.");

            api->SetGraphicsQueueIndex(graphicsQueueIndex);
            api->SetPresentQueueIndex(presentQueueIndex);

            api->InitDevice();

            vkGetDeviceQueue(api->GetDevice(), graphicsQueueIndex, 0, api->GetQueue());

            u32 formatCount;
            vkGetPhysicalDeviceSurfaceFormatsKHR(api->GetPhysicalDevice(), this->m_Surface, &formatCount, nullptr);

            if (formatCount == 0)
                throw Exception(Error::SYSTEM_ERROR, "Unable to get surface format! GetPhysicalDeviceSurfaceFormatsKHR Failure.");

            VkSurfaceFormatKHR* surfaceFormats = oallocat(VkSurfaceFormatKHR, formatCount, oSystemAllocator);
            vkGetPhysicalDeviceSurfaceFormatsKHR(api->GetPhysicalDevice(), this->m_Surface, &formatCount, surfaceFormats);

            if (formatCount == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED) {
                this->m_Format = VK_FORMAT_B8G8R8A8_UNORM;
            }
            else {
                this->m_Format = surfaceFormats[0].format;
            }

            this->m_ColorSpace = surfaceFormats[0].colorSpace;

            ofree(surfaceFormats, oSystemAllocator);
        }

        void vkGraphicsContext::SwapBuffers() {

        }

    }   // Splash

}   // Ocean
