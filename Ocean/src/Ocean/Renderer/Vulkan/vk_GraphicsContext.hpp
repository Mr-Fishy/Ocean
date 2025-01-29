#pragma once

/**
 * @file vk_GraphicsContext.hpp
 * @author Evan F.
 * @brief 
 * @date 2025-01-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Primitives/Array.hpp"

#include "Ocean/Renderer/GraphicsContext.hpp"

#include <glad/vulkan.h>

struct GLFWwindow;

namespace Ocean {

    namespace Splash {
    
        class vkGraphicsContext : public GraphicsContext {
        public:
            vkGraphicsContext() = delete;
            vkGraphicsContext(GLFWwindow* window);
            virtual ~vkGraphicsContext();

            virtual void Init() override final;
            
            virtual void SwapBuffers() override final;

        private:
            GLFWwindow* p_WindowHandle;

            VkSurfaceKHR m_Surface;

            VkSwapchainKHR m_Swapchain;
            DynamicArray<VkImage> m_SwapchainImages;

            VkFormat m_SwapchainFormat;
            VkExtent2D m_SwapchainExtent;

        };  // GraphicsContext

    }   // Splash

}   // Ocean
