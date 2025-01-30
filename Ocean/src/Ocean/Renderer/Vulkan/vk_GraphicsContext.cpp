#include "vk_GraphicsContext.hpp"

#include "Ocean/Primitives/Assert.hpp"
#include "Ocean/Renderer/RenderCommand.hpp"
#include "Ocean/Renderer/Vulkan/vk_RendererAPI.hpp"

// libs
#define GLFW_INCLUDE_NONE
#include <glad/vulkan.h>
#include <GLFW/glfw3.h>

namespace Ocean {

    namespace Splash {
    
        vkGraphicsContext::vkGraphicsContext(GLFWwindow* window) : p_WindowHandle(window), m_Surface(VK_NULL_HANDLE), m_Swapchain(VK_NULL_HANDLE), m_SwapchainImages(), m_SwapchainFormat(), m_SwapchainExtent() {
            OASSERTM(this->p_WindowHandle != nullptr, "Window Handle Is A nullptr!");
        }

        vkGraphicsContext::~vkGraphicsContext() {

        }

        void vkGraphicsContext::Init() {
            glfwCreateWindowSurface(static_cast<vkRendererAPI*>(RenderCommand::GetRenderer().get())->GetInstance(), this->p_WindowHandle, nullptr, &this->m_Surface);

            
        }

        void vkGraphicsContext::SwapBuffers() {

        }

    }   // Splash

}   // Ocean
