#pragma once

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


        private:
            VkSwapchainKHR m_Swapchain;

            VkSurfaceFormatKHR m_Format;
            VkExtent2D m_Extent;

            DynamicArray<SwapchainImage> m_Images;

            b8 m_IsValid = false;

        };  // vkSwapchain

    }   // Splash

}   // Ocean
