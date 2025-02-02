#include "vk_Instance.hpp"

#include "Ocean/Renderer/Vulkan/vk_Device.hpp"
#include "Ocean/Types/Bool.hpp"
#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/SmartPtrs.hpp"
#include "Ocean/Types/Strings.hpp"

#include "Ocean/Primitives/Array.hpp"
#include "Ocean/Primitives/Exceptions.hpp"
#include "Ocean/Primitives/Macros.hpp"
#include "Ocean/Primitives/Log.hpp"

#include "Ocean/Renderer/Vulkan/vk_Vulkan.hpp"

// std
#include <cstring>
#include <utility>

// libs
#include <glad/vulkan.h>
#include <GLFW/glfw3.h>

namespace Ocean {

    namespace Splash {

        OC_STATIC VKAPI_ATTR b32 VKAPI_CALL vkMessageCallback(
            VkFlags msgFlags, VkDebugReportObjectTypeEXT objType,
            u64 srcObject, sizet location,i32 msgCode,
            cstring pLayerPrefix, cstring pMsg, void* pUserData
        ) {
            switch (msgFlags) {
                case VK_DEBUG_REPORT_ERROR_BIT_EXT:
                    oprintret(CONSOLE_TEXT_RED("Vulkan Error: [%s] Code %d : %s"), pLayerPrefix, msgCode, pMsg);
                    return VK_TRUE;

                case VK_DEBUG_REPORT_WARNING_BIT_EXT:
                    oprintret(CONSOLE_TEXT_RED("Vulkan Error: [%s] Code %d : %s"), pLayerPrefix, msgCode, pMsg);
                    return VK_TRUE;
            }

            return VK_FALSE;
        }



        vkInstance::vkInstance() :
            m_Instance(VK_NULL_HANDLE),
        #ifdef OC_DEBUG
            m_DebugCallback(VK_NULL_HANDLE),
        #endif
            m_Extensions(0),
            m_Layers(0),
            m_Devices(0)
        {
            oprint("Initializing Vulkan Instance!\n");

            i32 gladVersion = gladLoaderLoadVulkan(nullptr, nullptr, nullptr);
            if (!gladVersion)
                throw Exception(Error::SYSTEM_ERROR, "Unable to load Vulkan symbols! gladLoad Failure.");

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

            // hasDebugCapabilites remains outside of an #ifdef as it may still be usefull to know if there is debug extensions available during release.
            //
            b8 hasDebugCapabilities = false;

            /** @todo Add release vs debug define to use in functionality for debug support if user requested during release. */

            for (const VkExtensionProperties& extension : availableExtensions) {
                if (std::strncmp(extension.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME, strlen(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)) == 0) {
                    hasDebugCapabilities = true;

                    break;
                }
            }

            if (hasDebugCapabilities) {
                this->m_Extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
                extensionCount++;
            }
            else
                oprint(CONSOLE_TEXT_YELLOW("%s is not available...Disabling debug utils messenger."), VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

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
                // then get all of the available requestedValidationLayers_Alt-ernatives.
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

            /** @todo Make pApplicationName, applicationVersion, pEngineName, and engineVersion defined variable dependent. */

            VkApplicationInfo appInfo {
                VK_STRUCTURE_TYPE_APPLICATION_INFO,
                nullptr,

                "Ocean App",
                VK_MAKE_VERSION(1, 0, 0),

                "Ocean Engine",
                VK_MAKE_VERSION(1, 0, 0),

                VK_MAKE_VERSION(1, 3, 0)
            };

            VkInstanceCreateInfo instanceInfo {
                VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                nullptr,
                0,

                &appInfo,

                layerCount,
                this->m_Layers.data(),

                extensionCount,
                this->m_Extensions.data()
            };

            vkCheck(
                vkCreateInstance(&instanceInfo, nullptr, &this->m_Instance)
            );

            GetDevices();

            // Load's all of the related Vulkan symbols given the instance and the physical device selected.
            //
            gladVersion = gladLoaderLoadVulkan(this->m_Instance, this->m_Devices[0]->GetPhysical(), nullptr);
            if (!gladVersion)
                throw Exception(Error::SYSTEM_ERROR, "Unable to reload Vulkan symbols with physical device!");

            // Setup Vulkan's debug callbacks given that it hasDebugCapabilities.
            //
            if (hasDebugCapabilities && false) { /** @todo FIX. */
                this->m_CreateCallback = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(this->m_Instance, "vkCreateDebugReportCallbackEXT"));
                this->m_DestroyCallback = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(this->m_Instance, "vkDestroyDebugReportCallbackEXT"));

                if (!this->m_CreateCallback)
                    throw Exception(Error::SYSTEM_ERROR, "Unable to find vkCreateDebugReportCallbackEXT. vkGetProcAddr Failure.");
                if (!this->m_DestroyCallback)
                    throw Exception(Error::SYSTEM_ERROR, "Unable to find vkDestroyDebugReportCallbackEXT. vkGetProcAddr Failure.");

                this->m_ReportMessage = reinterpret_cast<PFN_vkDebugReportMessageEXT>(vkGetInstanceProcAddr(this->m_Instance, "vkDebugReportMessageEXT"));

                if (!this->m_ReportMessage)
                    throw Exception(Error::SYSTEM_ERROR, "Unable to find vkDebugReportMessageEXT. vkGetProcAddr Failure.");

                VkDebugReportCallbackCreateInfoEXT messengerCreateInfo {
                    VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT,
                    nullptr,
                    VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT,
                    vkMessageCallback,
                    nullptr
                };

                vkCheck(
                    this->m_CreateCallback(this->m_Instance, &messengerCreateInfo, nullptr, &this->m_DebugCallback)
                );
            }
        }

        vkInstance::~vkInstance() {
            
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
                    throw Exception(Error::SYSTEM_ERROR, "");

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
