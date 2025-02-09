#pragma once

/**
 * @file vk_Pipeline.hpp
 * @author Evan F.
 * @brief 
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Renderer/Vulkan/vk_Framebuffer.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {
    
        class vkPipeline {
        public:
            vkPipeline(const vkFramebuffer& framebuffer);
            ~vkPipeline();



        private:
            const vkFramebuffer& c_Framebuffer;

            VkPipeline m_Pipeline;

        };  // vkPipeline

    }   // Splash

}   // Ocean
