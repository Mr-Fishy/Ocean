#pragma once

#include "Ocean/Types/SmartPtrs.hpp"
#include "Ocean/Types/Strings.hpp"

#include "Ocean/Primitives/Array.hpp"
#include "Ocean/Primitives/Macros.hpp"

#include "Ocean/Renderer/Vulkan/vk_Device.hpp"
#include "Ocean/Renderer/Vulkan/vk_Swapchain.hpp"

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

            OC_INLINE VkInstance Instance() const { return this->m_Instance; }

            OC_INLINE const Ref<vkDevice>& Device() { return this->m_Devices[0]; }

            const DynamicArray<cstring>& Extensions() { return this->m_Extensions; }

            OC_INLINE void InitSwapchain(VkSurfaceKHR surface) { this->m_Swapchain = MakeRef<vkSwapchain>(surface); };

            OC_INLINE const Ref<vkSwapchain>& Swapchain() { return this->m_Swapchain; }

        private:
            void GetDevices();

        private:
            OC_NO_COPY(vkInstance);

        private:
            VkInstance m_Instance;

            DynamicArray<cstring> m_Extensions;
            DynamicArray<cstring> m_Layers;

            DynamicArray<Ref<vkDevice>> m_Devices;

            Ref<vkSwapchain> m_Swapchain;

        };  // vkInstance

    }   // Splash

}   // Ocean
