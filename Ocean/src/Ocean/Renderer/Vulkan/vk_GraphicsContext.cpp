#include "vk_GraphicsContext.hpp"

#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Assert.hpp"

#include "Ocean/Renderer/Vulkan/vk_Instance.hpp"
#include "Ocean/Renderer/Vulkan/vk_Swapchain.hpp"

// libs
#define GLFW_INCLUDE_NONE
#include <glad/vulkan.h>
#include <GLFW/glfw3.h>

namespace Ocean {

    namespace Splash {
    
        vkGraphicsContext::vkGraphicsContext(GLFWwindow* window) : p_WindowHandle(window) {
            OASSERTM(this->p_WindowHandle != nullptr, "Window Handle Is A nullptr!");
        }

        vkGraphicsContext::~vkGraphicsContext() {

        }

        void vkGraphicsContext::Init() {
            glfwCreateWindowSurface(vkInstance::Get().Instance(), this->p_WindowHandle, nullptr, &this->m_Surface);

            this->m_Swapchain = MakeRef<vkSwapchain>(this->m_Surface);
        }

        void vkGraphicsContext::SwapBuffers() {

        }

    }   // Splash

}   // Ocean
