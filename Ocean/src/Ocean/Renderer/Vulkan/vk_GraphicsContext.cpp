#include "vk_GraphicsContext.hpp"

#include "Ocean/Primitives/Assert.hpp"

#include "Ocean/Renderer/Vulkan/vk_Instance.hpp"

// libs
#include <glad/vulkan.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Ocean {

    namespace Splash {
    
        vkGraphicsContext::vkGraphicsContext(GLFWwindow* window) :
            p_WindowHandle(window)
        {
            OASSERTM(this->p_WindowHandle != nullptr, "Window Handle Is A nullptr!");
        }

        vkGraphicsContext::~vkGraphicsContext() {

        }

        void vkGraphicsContext::Init() {
            VkSurfaceKHR surface;
            glfwCreateWindowSurface(vkInstance::Get().Instance(), this->p_WindowHandle, nullptr, &surface);

            vkInstance::Get().InitSwapchain(surface);
        }

        void vkGraphicsContext::SwapBuffers() {

        }

    }   // Splash

}   // Ocean
