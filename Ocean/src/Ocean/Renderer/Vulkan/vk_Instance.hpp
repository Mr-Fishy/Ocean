#pragma once

#include "Ocean/Types/SmartPtrs.hpp"
#include "Ocean/Types/Strings.hpp"

#include "Ocean/Primitives/Array.hpp"
#include "Ocean/Primitives/Macros.hpp"

#include "Ocean/Renderer/Vulkan/vk_Device.hpp"

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

        private:
            VkInstance m_Instance;

        #ifdef OC_DEBUG

            PFN_vkCreateDebugReportCallbackEXT m_CreateCallback;
            PFN_vkDestroyDebugReportCallbackEXT m_DestroyCallback;
            PFN_vkDebugReportMessageEXT m_ReportMessage;

            VkDebugReportCallbackEXT m_DebugCallback;

        #endif

            DynamicArray<cstring> m_Extensions;
            DynamicArray<cstring> m_Layers;

            DynamicArray<Ref<vkDevice>> m_Devices;

        private:
            void GetDevices();

        };  // vkInstance

    }   // Splash

}   // Ocean
