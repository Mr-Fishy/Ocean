#pragma once

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {
    
        class vkPipeline {
            vkPipeline();
            ~vkPipeline();

        private:
            VkPipeline m_Pipeline;

            

        };  // vkPipeline

    }   // Splash

}   // Ocean
