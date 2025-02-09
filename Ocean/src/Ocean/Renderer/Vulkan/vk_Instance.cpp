#include "vk_Instance.hpp"

#include "Ocean/Types/Bool.hpp"
#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/SmartPtrs.hpp"
#include "Ocean/Types/Strings.hpp"

#include "Ocean/Primitives/Array.hpp"
#include "Ocean/Primitives/Exceptions.hpp"
#include "Ocean/Primitives/Macros.hpp"
#include "Ocean/Primitives/Log.hpp"

#include "Ocean/Renderer/Vulkan/vk_Vulkan.hpp"
#include "Ocean/Renderer/Vulkan/vk_Device.hpp"
#include "vk_Command.hpp"

// std
#include <cstring>
#include <stdexcept>
#include <utility>

// libs
#include <glad/vulkan.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Ocean {

    namespace Splash {

        //
        // Debug callback code adapted from https://stackoverflow.com/questions/75573117/vkgetinstanceprocaddrinstance-vkcreatedebugutilsmessengerext-returns-null
        //

        VkResult CreateDebugUtilsMessengerExt(
            VkInstance instance,
            const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkDebugUtilsMessengerEXT* pDebugMessenger
        ) {
            auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
            
            if (func)
                return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
            else
                return VK_ERROR_EXTENSION_NOT_PRESENT;
        }

        void DestroyDebutUtilsMessengerExt(
            VkInstance instance,
            VkDebugUtilsMessengerEXT debugMessenger,
            const VkAllocationCallbacks* pAllocator
        ) {
            auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));

            if (func)
                func(instance, debugMessenger, pAllocator);
        }

        OC_STATIC VKAPI_ATTR b32 VKAPI_CALL vkMessageCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            OC_UNUSED VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            OC_UNUSED void* pUserData
        ) {
            switch (messageSeverity) {
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
                    // oprint("Vulkan DIAGNOSTIC:\t %s\n", pCallbackData->pMessage);
                    return VK_TRUE;

                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                    oprint(CONSOLE_TEXT_CYAN("Vulkan INFO:\t %s\n"), pCallbackData->pMessage);
                    return VK_TRUE;

                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                    oprint(CONSOLE_TEXT_YELLOW("Vulkan WARNING:\t %s\n"), pCallbackData->pMessage);
                    return VK_TRUE;

                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                    oprint(CONSOLE_TEXT_RED("Vulkan ERROR:\t %s\n"), pCallbackData->pMessage);
                    return VK_TRUE;

                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
                    break;
            }

            oprint(CONSOLE_TEXT_RED("UNKOWN Vulkan Message!\n"));

            return VK_FALSE;
        }



        vkInstance::vkInstance() :
            m_Instance(VK_NULL_HANDLE),
            m_Extensions(0),
            m_Layers(0),
            m_Devices(0)
        {
            i32 gladVersion = gladLoaderLoadVulkan(nullptr, nullptr, nullptr);
            if (!gladVersion)
                throw Exception(Error::SYSTEM_ERROR, "Unable to load Vulkan symbols! gladLoad Failure.");

            // ================================ VALIDATION LAYERS ================================
            //
            // Collect all of the validation layers required for the Vulkan instance, aka debug layers and Vulkan checks.
            //
            u32 layerCount;
            vkCheck(
                vkEnumerateInstanceLayerProperties(&layerCount, nullptr)
            );

            if (layerCount > 0) {
                DynamicArray<VkLayerProperties> availableValidationLayers(layerCount);
                vkCheck(
                    vkEnumerateInstanceLayerProperties(&layerCount, availableValidationLayers.data())
                );

                cstring requestedValidationLayers[] = {
                    "VK_LAYER_KHRONOS_validation"
                };
                layerCount = ArraySize(requestedValidationLayers);

                // Checks if the system's Vulkan driver supports the requestedValidationLayers.
                //
                u32 foundLayers = 0;
                for (u32 i = 0; i < layerCount; i++) {
                    for (const VkLayerProperties& layer : availableValidationLayers)
                        if (!std::strcmp(requestedValidationLayers[i], layer.layerName))
                            foundLayers++;
                    
                    if (foundLayers != i + 1)
                        oprint(CONSOLE_TEXT_RED("Unable to find Vulkan layer: %s\n"), requestedValidationLayers[i]);
                }

                // If it did not find all of the requestedValidationLayers,
                // then get all of the available requestedValidationLayers_Alt's.
                // 
                // If all of the requestedValidationLayers were found, then they will be added to m_Layers.
                //
                if (layerCount != foundLayers) {
                    cstring requestedValidationLayers_Alt[] = {
                        "VK_LAYER_LUNARG_parameter_validation",
                        "VK_LAYER_LUNARG_object_tracker",
                        "VK_LAYER_LUNARG_image",
                        "VK_LAYER_LUNARG_core_validation",
                        "VK_LAYER_LUNARG_swapchain",
                        "VK_LAYER_GOOGLE_threading",
                        "VK_LAYER_GOOGLE_unique_objects"
                    };
                    layerCount = ArraySize(requestedValidationLayers_Alt);

                    // If an alternative validation layer is supported, then add it to m_Layers,
                    // as it can be assumed that layer support may be limited.
                    //
                    for (u32 i = 0; i < layerCount; i++) {
                        for (const VkLayerProperties& layer : availableValidationLayers)
                            if (!std::strcmp(requestedValidationLayers_Alt[i], layer.layerName))
                                this->m_Layers.emplace_back(layer.layerName);
                    }

                    // Set the layerCount to the resulting layers found for later use.
                    //
                    layerCount = this->m_Layers.size();
                }
                else {
                    for (const cstring& layerName : requestedValidationLayers)
                        this->m_Layers.emplace_back(layerName);
                }
            }

            // ================================ EXTENSIONS ================================
            //
            // Collect all of the extensions required for the Vulkan instance, aka platform and debugging support.
            //
            u32 extensionCount;
            vkCheck(
                vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr)
            );

            DynamicArray<VkExtensionProperties> availableExtensions(extensionCount);
            vkCheck(
                vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data())
            );

            // extensionCount is re-used for required extensionCount rather than available extensionCount
            //
            cstring* glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);

            for (u32 i = 0; i < extensionCount; i++)
                this->m_Extensions.emplace_back(glfwExtensions[i]);

            // hasDebugCapabilites should remain outside of an #ifdef as it may still be usefull to know if there is debug extensions available during release.
            //
            b8 hasDebugCapabilities = false;

            /** @todo Add release vs debug define to use in functionality for debug support if user requested during release. */

            for (const VkExtensionProperties& extension : availableExtensions) {
                if (std::strncmp(extension.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME, strlen(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)) == 0) {
                    hasDebugCapabilities = true;

                    break;
                }
            }

            if (hasDebugCapabilities && GLAD_VK_EXT_debug_report) {
                this->m_Extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
                extensionCount++;
            }
            else
                oprint(CONSOLE_TEXT_YELLOW("%s is not available...Disabling debug utils messenger."), VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

            /** @todo Make pApplicationName, applicationVersion, pEngineName, and engineVersion defined variable dependent. */

            // ================================ INSTANCE CREATION ================================
            //
            VkApplicationInfo appInfo {
                VK_STRUCTURE_TYPE_APPLICATION_INFO,
                nullptr,

                "Ocean App",
                VK_MAKE_VERSION(1, 0, 0),

                "Ocean Engine",
                VK_MAKE_VERSION(1, 0, 0),

                VK_API_VERSION_1_3
            };

            for (const cstring& layer : this->m_Layers)
                oprint(CONSOLE_TEXT_CYAN("Enabling Layer: %s\n"), layer);

            for (const cstring& ext : this->m_Extensions)
                oprint(CONSOLE_TEXT_CYAN("Enabling Extension: %s\n"), ext);

            VkInstanceCreateInfo instanceInfo {
                VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                nullptr,
                0,

                &appInfo,

                0,
                nullptr,

                extensionCount,
                this->m_Extensions.data()
            };

            // ================================ DEBUG INFO ================================
            //
            VkDebugUtilsMessengerCreateInfoEXT messengerInfo { };
            if (hasDebugCapabilities) {
                instanceInfo.enabledLayerCount = layerCount;
                instanceInfo.ppEnabledLayerNames = this->m_Layers.data();

                messengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

                messengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

                messengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

                messengerInfo.pfnUserCallback = vkMessageCallback;

                instanceInfo.pNext = &messengerInfo;
            }

            // ================================ INSTANCE CREATION ================================
            //
            vkCheck(
                vkCreateInstance(&instanceInfo, nullptr, &this->m_Instance)
            );

            // ================================ DEVICE CREATION ================================
            //
            GetDevices();

            // Load's all of the related Vulkan symbols given the instance and the physical device selected.
            //
            gladVersion = gladLoaderLoadVulkan(this->m_Instance, this->m_Devices[0]->GetPhysical(), nullptr);
            if (!gladVersion)
                throw Exception(Error::SYSTEM_ERROR, "Unable to reload Vulkan symbols with physical device!");

            if (GLAD_VK_KHR_swapchain) {
                this->m_Devices[0]->AddDeviceExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
            }
            else
                throw Exception(Error::SYSTEM_ERROR, "Failed to find extension: " VK_KHR_SWAPCHAIN_EXTENSION_NAME ". vkCreateInstance Failure.");
        }

        vkInstance::~vkInstance() {
            
        }

        void vkInstance::Prepare() {
            this->m_CommandPool = MakeScope<vkCommandPool>(this->m_Swapchain->GraphicsQueueIndex());

            this->m_CommandPool->CreateBuffer("DrawCmd", true);


        }

        void vkInstance::Cleanup() {
            this->m_CommandPool.reset();
        }



        void vkInstance::GetDevices() {
            u32 gpuCount;
            vkCheck(
                vkEnumeratePhysicalDevices(this->m_Instance, &gpuCount, nullptr)
            );

            if (gpuCount < 1)
                throw Exception(Error::SYSTEM_ERROR, "Unable to find useable Vulkan devices!");

            DynamicArray<VkPhysicalDevice> physicalDevices(gpuCount);
            vkCheck(
            vkEnumeratePhysicalDevices(this->m_Instance, &gpuCount, physicalDevices.data())
            );

            for (const VkPhysicalDevice& gpu : physicalDevices) {
                if (!gladLoaderLoadVulkan(this->m_Instance, gpu, nullptr))
                    throw Exception(Error::SYSTEM_ERROR, "Unable to reload Vulkan symbols with a gpu! gladLoad Failure.");

                Ref<vkDevice> device = MakeRef<vkDevice>(gpu);

                if (device->GetDeviceScore() > 0)
                    this->m_Devices.emplace_back(device);
            }

            for (u32 i = 1; i < this->m_Devices.size(); i++) {
                if (this->m_Devices[i]->GetDeviceScore() > this->m_Devices[i - 1]->GetDeviceScore())
                    std::swap(this->m_Devices[i], this->m_Devices[i - 1]);
            }
        }

    }   // Splash

}   // Ocean
