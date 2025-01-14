#include "vk_GraphicsContext.hpp"

// libs
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/vulkan.h>

namespace Ocean {

    namespace Shrimp {
    
        vkGraphicsContext::vkGraphicsContext(GLFWwindow* window) : p_WindowHandle(window) {

        }

        vkGraphicsContext::~vkGraphicsContext() {

        }

        void vkGraphicsContext::Init() {

        }
        
        void vkGraphicsContext::SwapBuffers() {
            
        }

    }   // Shrimp

}   // Ocean
