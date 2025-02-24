#include "vk_GraphicsContext.hpp"

#include "Ocean/Renderer/GraphicsContext.hpp"
#include "Ocean/Renderer/Vulkan/vk_Instance.hpp"
#include "Ocean/Renderer/Vulkan/vk_Swapchain.hpp"
#include "Ocean/Types/SmartPtrs.hpp"

// libs
#include <glad/vulkan.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Ocean {

    namespace Splash {
    
        vkGraphicsContext::vkGraphicsContext(GLFWwindow* window) :
            GraphicsContext(window)
        {

        }

        vkGraphicsContext::~vkGraphicsContext() {

        }

        void vkGraphicsContext::Init() {
            VkSurfaceKHR surface;
            glfwCreateWindowSurface(vkInstance::Get().Instance(), this->p_WindowHandle, nullptr, &surface);

            vkInstance::Get().Swapchain() = MakeRef<vkSwapchain>(surface);
        }

        void vkGraphicsContext::SwapBuffers() {

        }

    }   // Splash

}   // Ocean
