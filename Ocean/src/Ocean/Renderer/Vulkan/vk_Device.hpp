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
#include "Ocean/Primitives/DynamicArray.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {

        class vkDevice {
        public:
            vkDevice() = default;
            /**
             * @brief Construct a new vkDevice object with the given GPU.
             * 
             * @param gpu The physical device to build the logical device with.
             */
            vkDevice(VkPhysicalDevice gpu);
            ~vkDevice();

            /**
             * @brief Get's the Vulkan physical device handle.
             * 
             * @return VkPhysicalDevice 
             */
            OC_INLINE VkPhysicalDevice Physical() const { return this->m_gpu; }
            /**
             * @brief Get's the Vulkan logical device handle.
             * 
             * @return VkDevice 
             */
            OC_INLINE VkDevice Logical() const { return this->m_Device; }

            /**
             * @brief Add's a device extension to the logical device.
             * @note This should only be used before initializing the logical device.
             * 
             * @param extension The name of the extension per the Vulkan spec.
             */
            OC_INLINE void AddDeviceExtension(cstring extension) { this->m_Extensions.EmplaceBack(extension); }

            /**
             * @brief Initializes the logical Vulkan device.
             * 
             * @param queueInfo The queue family info to use with the device.
             */
            void InitLogicalDevice(VkDeviceQueueCreateInfo& queueInfo);

            /**
             * @brief Get's the device score in terms of feature support and hardware capabilities.
             * 
             * @return i32
             */
            i32 GetDeviceScore();

        private:
            OC_NO_COPY(vkDevice);

        private:
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
