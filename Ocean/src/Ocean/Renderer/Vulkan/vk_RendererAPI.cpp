#include "vk_RendererAPI.hpp"

#include "GLFW/glfw3.h"
#include "Ocean/Renderer/RendererAPI.hpp"
#include "Ocean/Types/FloatingPoints.hpp"
#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Exceptions.hpp"
#include "Ocean/Primitives/Macros.hpp"
#include "Ocean/Primitives/Log.hpp"
#include "Ocean/Primitives/Memory.hpp"

#include "Ocean/Renderer/VertexArray.hpp"
#include "Ocean/Types/Strings.hpp"

// std
#include <cstring>

// libs
#define GLAD_VULKAN_IMPLEMENTATION
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {

        OC_UNUSED OC_STATIC VKAPI_ATTR b32 VKAPI_CALL vkMessageCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT severity,
            OC_UNUSED VkDebugUtilsMessageTypeFlagsEXT type,
            const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
            OC_UNUSED void* userData
        ) {
            switch (severity) {
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                    oprint(CONSOLE_TEXT_RED("\n%s\n"), callbackData->pMessage);
                    return VK_TRUE;

                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                    oprint(CONSOLE_TEXT_RED("\n%s\n"), callbackData->pMessage);
                    return VK_TRUE;

                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                    oprint(CONSOLE_TEXT_GREEN("\n%s\n"), callbackData->pMessage);
                    return VK_TRUE;

                case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
                    oprint(CONSOLE_TEXT_YELLOW("\n%s\n"), callbackData->pMessage);
                    return VK_TRUE;

                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
                    oprint(CONSOLE_TEXT_BLUE("\n%s\n"), callbackData->pMessage);
            }

            return VK_FALSE;
        }

        OC_STATIC b32 vkCheckLayers(u32 count, cstring* names, u32 layerCount, VkLayerProperties* layers) {
            b32 result = true;

            for (u32 i = 0; i < count; i++) {
                b32 found = false;
                
                for (u32 j = 0; j < layerCount; j++) {
                    if (!std::strcmp(names[i], layers[j].layerName)) {
                        found = true;

                        break;
                    }
                }

                if (!found) {
                    oprint(CONSOLE_TEXT_RED("Cannot find Vulkan layer: %s\n"), names[i]);

                    result = false;
                }
            }

            return result;
        }

        

        // https://github.com/Dav1dde/glad/blob/glad2/example/c/vulkan_tri_glfw/vulkan_tri_glfw.c#L2082

        // https://github.com/KhronosGroup/Vulkan-Samples/blob/main/samples/api/hello_triangle_1_3/hello_triangle_1_3.h
        // https://github.com/KhronosGroup/Vulkan-Samples/blob/main/samples/api/hello_triangle_1_3/hello_triangle_1_3.cpp

        vkRendererAPI::vkRendererAPI() :
            m_Instance(VK_NULL_HANDLE),
        #ifdef OC_DEBUG
            m_CreateDebugCallback(VK_NULL_HANDLE),
            m_DestroyDebugCallback(VK_NULL_HANDLE),
            m_MessageCallback(VK_NULL_HANDLE),
        #endif
            m_gpu(VK_NULL_HANDLE),
            m_gpuProperties(),
            m_gpuFeatures(),
            m_gpuMemory(),
            m_Device(VK_NULL_HANDLE),
            m_Extensions(0),
            m_Layers(0),
            m_Queue(VK_NULL_HANDLE),
            m_QueueProperties(0),
            m_GraphicsQueueIndex(u32_max),
            m_PresentQueueIndex(u32_max),
            m_CommandPool(VK_NULL_HANDLE) 
        {

        }

        vkRendererAPI::~vkRendererAPI() {

        }

        void vkRendererAPI::Init() {
            /** @todo Add graphics specific exception errors to be more descriptive. */

            int gladVulkanVersion = gladLoaderLoadVulkan(nullptr, nullptr, nullptr);

            if (!gladVulkanVersion)
                throw Exception(Error::SYSTEM_ERROR, "Unable to load Vulkan symbols!");

            if (!ValidateLayers())
                throw Exception(Error::SYSTEM_ERROR, "Failed to validate Vulkan layers!");

            u32 extensionCount;
            cstring* requiredExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);

            if (!requiredExtensions)
                throw Exception(Error::SYSTEM_ERROR, "Failed to get required surface layers!");

        #ifdef OC_DEBUG

            extensionCount++;

        #endif

            this->m_Extensions.resize(extensionCount);

            for (u32 i = 0; i < extensionCount; i++)
                this->m_Extensions.emplace_back(requiredExtensions[i]);

        #ifdef OC_DEBUG

            this->m_Extensions.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

        #endif

            VkApplicationInfo appInfo = { };
            appInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            appInfo.pNext = nullptr;
            appInfo.pApplicationName = "App"; /** @todo Change to get actual Application name. */
            appInfo.applicationVersion = 0; /** @todo Implement Application level versioning. */
            appInfo.pEngineName = "Ocean Engine";
            appInfo.engineVersion = 0; /** @todo Add CMAKE defined version. */
            appInfo.apiVersion = VK_API_VERSION_1_3;

            VkInstanceCreateInfo instInfo = { };
            instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            instInfo.pNext = nullptr;
            instInfo.pApplicationInfo = &appInfo;
            instInfo.enabledLayerCount = this->m_Layers.size();
            instInfo.ppEnabledLayerNames = this->m_Layers.data();
            instInfo.enabledExtensionCount = this->m_Extensions.size();
            instInfo.ppEnabledExtensionNames = this->m_Extensions.data();

            switch (vkCreateInstance(&instInfo, nullptr, &this->m_Instance)) {
                case VK_SUCCESS:
                    break;

                case VK_ERROR_INCOMPATIBLE_DRIVER:
                    throw Exception(Error::SYSTEM_ERROR, "Cannot find a compatible Vulkan driver. vkCreateInstance() Failure.");

                case VK_ERROR_EXTENSION_NOT_PRESENT:
                    throw Exception(Error::SYSTEM_ERROR, "Cannot find a specified extension. vkCreateInstance Failure.");

                default:
                    throw Exception(Error::SYSTEM_ERROR, "vkCreateInstance Failure.");
            }

            u32 gpuCount = 0;
            if (!vkEnumeratePhysicalDevices(this->m_Instance, &gpuCount, nullptr) || gpuCount == 0)
                throw Exception(Error::SYSTEM_ERROR, "Unable to get useable physical GPU devices!");

            VkPhysicalDevice* physicalDevices = oallocat(VkPhysicalDevice, gpuCount, oSystemAllocator);

            if (!vkEnumeratePhysicalDevices(this->m_Instance, &gpuCount, physicalDevices))
                throw Exception(Error::SYSTEM_ERROR, "Failed to get physical GPU devices!");

            /** @todo GPU selection for Vulkan instance. */

            this->m_gpu = physicalDevices[0];

            ofree(physicalDevices, oSystemAllocator);
            
            if (!(gladVulkanVersion = gladLoaderLoadVulkan(this->m_Instance, this->m_gpu, nullptr)))
                throw Exception(Error::SYSTEM_ERROR, "Failed to reload Vulkan symbols with instance!");

            if (GLAD_VK_KHR_swapchain)
                this->m_Extensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
            else
                throw Exception(Error::SYSTEM_ERROR, "Unable to find the " VK_KHR_SWAPCHAIN_EXTENSION_NAME " extension.");

        #ifdef OC_DEBUG

            this->m_CreateDebugCallback = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(this->m_Instance, "vkCreateDebugReportCallbackEXT"));

            if (!this->m_CreateDebugCallback)
                throw Exception(Error::SYSTEM_ERROR, "Unable to find vkCreateDebugReportCallbackEXT. vkGetProcAddr Failure.");

            this->m_DestroyDebugCallback = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(this->m_Instance, "vkDestroyDebugReportCallbackEXT"));

            if (!this->m_CreateDebugCallback)
                throw Exception(Error::SYSTEM_ERROR, "Unable to find vkDestroyDebugReportCallbackEXT. vkGetProcAddr Failure.");

            this->m_MessageCallback = reinterpret_cast<PFN_vkDebugReportMessageEXT>(vkGetInstanceProcAddr(this->m_Instance, "vkDebugReportMessageEXT"));

            if (!this->m_CreateDebugCallback)
                throw Exception(Error::SYSTEM_ERROR, "Unable to find vkDebugReportMessageEXT. vkGetProcAddr Failure.");

        #endif

            vkGetPhysicalDeviceProperties(this->m_gpu, &this->m_gpuProperties);

            u32 queueCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(this->m_gpu, &queueCount, nullptr);

            this->m_QueueProperties.resize(queueCount);
            vkGetPhysicalDeviceQueueFamilyProperties(this->m_gpu, &queueCount, this->m_QueueProperties.data());

            if (this->m_QueueProperties.empty())
                throw Exception(Error::SYSTEM_ERROR, "Unable to get physical device queues!");

            vkGetPhysicalDeviceFeatures(this->m_gpu, &this->m_gpuFeatures);

            vkGetPhysicalDeviceMemoryProperties(this->m_gpu, &this->m_gpuMemory);

            /** @todo Add separate queue support, synchronization, and tracking for queue submitions. */
        }

        void vkRendererAPI::SetViewport(u32 x, u32 y, u32 w, u32 h) {
            
        }

        void vkRendererAPI::SetClearColor(const glm::vec4& color) {

        }

        void vkRendererAPI::Clear() {

        }

        void vkRendererAPI::DrawIndexed(const Ref<VertexArray>& array, u32 indexCount) {
            u32 count = indexCount ? indexCount : array->GetIndexBuffer()->GetCount();

        }

        void vkRendererAPI::InitDevice() {
            f32 queuePriorities[1] = {
                0.0f
            };

            VkDeviceQueueCreateInfo queueInfo { };
            queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfo.pNext = nullptr;
            queueInfo.queueFamilyIndex = this->m_GraphicsQueueIndex;
            queueInfo.queueCount = 1;
            queueInfo.pQueuePriorities = queuePriorities;

            VkPhysicalDeviceFeatures features;
            if (this->m_gpuFeatures.shaderClipDistance)
                features.shaderClipDistance = VK_TRUE;

            VkDeviceCreateInfo deviceInfo { };
            deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            deviceInfo.pNext = nullptr;
            deviceInfo.queueCreateInfoCount = 1;
            deviceInfo.pQueueCreateInfos = &queueInfo;
            deviceInfo.enabledLayerCount = 0;
            deviceInfo.ppEnabledLayerNames = nullptr;
            deviceInfo.enabledExtensionCount = this->m_Extensions.size();
            deviceInfo.ppEnabledExtensionNames = this->m_Extensions.data();
            deviceInfo.pEnabledFeatures = &features;

            if (!vkCreateDevice(this->m_gpu, &deviceInfo, nullptr, &this->m_Device))
                throw Exception(Error::SYSTEM_ERROR, "Unable to create Vulkan device! InitDevice Failure.");

            u32 gladVulkanVersion = gladLoaderLoadVulkan(this->m_Instance, this->m_gpu, this->m_Device);
            if (!gladVulkanVersion)
                throw Exception(Error::SYSTEM_ERROR, "Unable to reload Vulkan symbols with Device! gladLoad Failure.");
        }



        b32 vkRendererAPI::ValidateLayers() {
            u32 layerCount = 0;
            if (vkEnumerateInstanceLayerProperties(&layerCount, nullptr) || layerCount == 0)
                return false; 

            VkLayerProperties* instanceLayers = oallocat(VkLayerProperties, layerCount, oSystemAllocator);
            
            if (vkEnumerateInstanceLayerProperties(&layerCount, instanceLayers))
                return false;

            cstring validationLayers[] = {
                "VK_LAYER_LUNARG_standard_validation"
            };

            cstring alternativeLayers[] = {
                "VK_LAYER_GOOGLE_threading",
                "VK_LAYER_LUNARG_parameter_validation",
                "VK_LAYER_LUNARG_object_tracker",
                "VK_LAYER_LUNARG_image",
                "VK_LAYER_LUNARG_core_validation",
                "VK_LAYER_LUNARG_swapchain",
                "VK_LAYER_GOOGLE_unique_objects"
            };

            if (vkCheckLayers(ArraySize(validationLayers), validationLayers, layerCount, instanceLayers)) {
                this->m_Layers.resize(ArraySize(validationLayers));
                
                for (cstring layer : validationLayers)
                    this->m_Layers.emplace_back(layer);
            }
            else if (vkCheckLayers(ArraySize(alternativeLayers), alternativeLayers, layerCount, instanceLayers)) {
                this->m_Layers.resize(ArraySize(alternativeLayers));

                for (cstring layer : validationLayers)
                    this->m_Layers.emplace_back(layer);
            }

            ofree(instanceLayers, oSystemAllocator);
            return true;
        }

    }   // Splash

}   // Ocean
