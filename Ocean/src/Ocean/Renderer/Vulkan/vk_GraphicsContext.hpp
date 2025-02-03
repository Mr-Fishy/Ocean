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

#include "Ocean/Primitives/Macros.hpp"

#include "Ocean/Renderer/GraphicsContext.hpp"

#include "Ocean/Renderer/Vulkan/vk_Swapchain.hpp"

// libs
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
            OC_NO_COPY(vkGraphicsContext);

            GLFWwindow* p_WindowHandle;

            VkSurfaceKHR m_Surface;

            Ref<vkSwapchain> m_Swapchain;

        };  // vkGraphicsContext

    }   // Splash

}   // Ocean
