#pragma once

#include "Ocean/Primitives/Array.hpp"
#include "Ocean/Primitives/Macros.hpp"

#include "Ocean/Renderer/Vulkan/vk_Device.hpp"
#include "Ocean/Types/Bool.hpp"

// std
#include <mutex>

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {
    
        class vkInstance {
        public:
            vkInstance();
            ~vkInstance();

            OC_GET_SINGLETON(vkInstance);

        private:
            VkInstance m_Instance;

        #ifdef OC_DEBUG

            VkDebugUtilsMessengerEXT m_DebugCallback;

        #endif

            DynamicArray<vkDevice> m_Devices;

        };  // vkInstance

    }   // Splash

}   // Ocean
