#pragma once

/**
 * @file vk_Swapchain.hpp
 * @author Evan F.
 * @brief 
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Types/Bool.hpp"

#include "Ocean/Primitives/Array.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {
    
        class vkSwapchain {
        private:
            struct SwapchainImage {
                VkImage image;
                VkImageView view;

            };  // SwapchainImage

        public:
            vkSwapchain(VkSurfaceKHR surface);
            ~vkSwapchain();

        private:
            VkSwapchainKHR m_Swapchain;

            VkQueue m_Queue;
            u32 m_GraphicsQueueIndex;
            u32 m_PresentQueueIndex;

            VkFormat m_Format;
            VkColorSpaceKHR m_ColorSpace;
            VkExtent2D m_Extent;

            DynamicArray<SwapchainImage> m_Images;

            b8 m_IsValid = false;

        };  // vkSwapchain

    }   // Splash

}   // Ocean
