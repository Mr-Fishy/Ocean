#pragma once

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {
    
        class vkInstance {
        public:
            vkInstance();
            ~vkInstance();

        private:
            VkInstance m_Instance;

        };  // vkInstance

    }   // Splash

}   // Ocean
