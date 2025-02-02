#include "vk_Device.hpp"

#include "Ocean/Primitives/Log.hpp"

#include "Ocean/Renderer/Vulkan/vk_Vulkan.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {

        vkDevice::vkDevice(VkPhysicalDevice gpu) :
            m_gpu(gpu),
            m_gpuFeatures(0),
            m_gpuProperties(),
            m_gpuMemory(),
            m_Device(),
            m_Queue(),
            m_GraphicsQueueIndex(),
            m_PresentQueueIndex()
        {
            vkGetPhysicalDeviceFeatureBits(this->m_gpu, &this->m_gpuFeatures);

            vkGetPhysicalDeviceProperties(this->m_gpu, &this->m_gpuProperties);

            vkGetPhysicalDeviceMemoryProperties(this->m_gpu, &this->m_gpuMemory);
        }

        vkDevice::~vkDevice() {
            
        }

        i32 vkDevice::GetDeviceScore() {
            i32 score = 0;

            if (this->m_gpuProperties.apiVersion < VK_API_VERSION_1_3)
                return -1;

            if (GLAD_VK_KHR_swapchain)
                score++;
            else
                return -1;

            if (this->m_gpuProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                score += 10;

            oprint("Vulkan Device Info\n");
            oprint("\tRenderer: %s\n", this->m_gpuProperties.deviceName);
            oprint("\tVersion: %i.%i.%i\n", VK_VERSION_MAJOR(this->m_gpuProperties.apiVersion), VK_VERSION_MINOR(this->m_gpuProperties.apiVersion), VK_VERSION_PATCH(this->m_gpuProperties.apiVersion));
            oprint("\tRating: %i\n", score);

            return score;
        }

    }   // Splash

}   // Ocean
