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

#include "Ocean/Primitives/Macros.hpp"

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

            VkDevice m_Device;

        };  // vkDevice

    }   // Splash

}   // Ocean
