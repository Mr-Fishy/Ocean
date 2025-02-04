#include "vk_Device.hpp"

#include "Ocean/Primitives/Array.hpp"
#include "Ocean/Types/Integers.hpp"

#include "Ocean/Primitives/Log.hpp"
#include "Ocean/Primitives/Exceptions.hpp"

#include "Ocean/Renderer/Vulkan/vk_Vulkan.hpp"
#include "Ocean/Renderer/Vulkan/vk_Instance.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {

        vkDevice::vkDevice(VkPhysicalDevice gpu) :
            m_gpu(gpu),
            m_gpuFeatures(0),
            m_gpuProperties(),
            m_gpuMemory(),
            m_Device(VK_NULL_HANDLE)
        {
            vkGetPhysicalDeviceFeatureBits(this->m_gpu, &this->m_gpuFeatures);

            vkGetPhysicalDeviceProperties(this->m_gpu, &this->m_gpuProperties);

            vkGetPhysicalDeviceMemoryProperties(this->m_gpu, &this->m_gpuMemory);
        }

        vkDevice::~vkDevice() {
            
        }

        void vkDevice::InitLogicalDevice(VkDeviceQueueCreateInfo& queueInfo) {
            VkPhysicalDeviceFeatures features { };
            if (this->m_gpuFeatures & VkPhysicalDeviceFeatureBits::ShaderClipDistance)
                features.shaderClipDistance = VK_TRUE;

            const DynamicArray<cstring>& extensions = vkInstance::Get().Extensions();

            for (const cstring ext : extensions)
                oprint("Requested Extension: %s\n", ext);

            VkDeviceCreateInfo deviceInfo {
                VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                nullptr,
                0,
                1,
                &queueInfo,
                0,
                nullptr,
                static_cast<u32>(extensions.size()),
                extensions.data(),
                &features
            };

            vkCheck(
                vkCreateDevice(this->m_gpu, &deviceInfo, nullptr, &this->m_Device)
            );

            if (!gladLoaderLoadVulkan(vkInstance::Get().Instance(), this->m_gpu, this->m_Device))
                throw Exception(Error::SYSTEM_ERROR, "Unable to reload Vulkan symbols with logical device! gladLoad Failure.");
        }

        i32 vkDevice::GetDeviceScore(const DynamicArray<cstring>& extensions) {
            i32 score = 0;

            if (this->m_gpuProperties.apiVersion < VK_API_VERSION_1_3)
                return -1;

            if (GLAD_VK_KHR_swapchain)
                score++;
            else
                return -1;

            if (this->m_gpuProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                score += 10;

            for (const cstring& ext : extensions) {
                
            }

            oprint("Vulkan Device Info\n");
            oprint("\tRenderer: %s\n", this->m_gpuProperties.deviceName);
            oprint("\tVersion: %i.%i.%i\n", VK_VERSION_MAJOR(this->m_gpuProperties.apiVersion), VK_VERSION_MINOR(this->m_gpuProperties.apiVersion), VK_VERSION_PATCH(this->m_gpuProperties.apiVersion));
            oprint("\tRating: %i\n", score);

            return score;
        }

    }   // Splash

}   // Ocean
