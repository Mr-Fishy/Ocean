#pragma once

/**
 * @file vk_Device.hpp
 * @author Evan F.
 * @brief 
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/Strings.hpp"

#include "Ocean/Primitives/Macros.hpp"
#include "Ocean/Primitives/Array.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {

        class vkDevice {
        public:
            vkDevice() = default;
            vkDevice(VkPhysicalDevice gpu);
            ~vkDevice();

            OC_INLINE VkPhysicalDevice GetPhysical() const { return this->m_gpu; }
            OC_INLINE VkDevice GetLogical() const { return this->m_Device; }

            OC_INLINE void AddDeviceExtension(cstring extension) { this->m_Extensions.emplace_back(extension); }

            void InitLogicalDevice(VkDeviceQueueCreateInfo& queueInfo);

            i32 GetDeviceScore();

        private:
            OC_NO_COPY(vkDevice);

            /* --- */

            VkPhysicalDevice m_gpu;
            /** @todo Figure out a more compact way to store the gpuProperties and gpuMemory. */
            u64 m_gpuFeatures;
            VkPhysicalDeviceProperties m_gpuProperties;
            VkPhysicalDeviceMemoryProperties m_gpuMemory;

            DynamicArray<cstring> m_Extensions;

            VkDevice m_Device;

        };  // vkDevice

    }   // Splash

}   // Ocean
