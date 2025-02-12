#pragma once

/**
 * @file vk_Swapchain.hpp
 * @author Evan F.
 * @brief 
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Primitives/Array.hpp"
#include "Ocean/Primitives/Macros.hpp"

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

            OC_INLINE u32 GraphicsQueueIndex() const { return this->m_GraphicsQueueIndex; }
            OC_INLINE u32 PresentQueueIndex() const { return this->m_PresentQueueIndex; }

            OC_INLINE const VkExtent2D& Extent() const { return this->m_Extent; }

            OC_INLINE VkFormat Format() const { return this->m_Format; }

        private:
            void CreateSwapchain();

        private:
            VkSurfaceKHR m_Surface;
            VkSwapchainKHR m_Swapchain;

            VkQueue m_Queue;
            u32 m_GraphicsQueueIndex;
            u32 m_PresentQueueIndex;

            VkFormat m_Format;
            VkColorSpaceKHR m_ColorSpace;
            VkExtent2D m_Extent;

            DynamicArray<SwapchainImage> m_Images;

        };  // vkSwapchain

    }   // Splash

}   // Ocean
