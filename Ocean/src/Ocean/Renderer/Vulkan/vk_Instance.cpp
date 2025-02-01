#include "vk_Instance.hpp"

#include "Ocean/Types/Bool.hpp"
#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/Strings.hpp"

#include "Ocean/Primitives/Array.hpp"
#include "Ocean/Primitives/Exceptions.hpp"
#include "Ocean/Primitives/Macros.hpp"
#include "Ocean/Primitives/Log.hpp"

#include "Ocean/Renderer/Vulkan/vk_Utils.hpp"

// std
#include <cstring>

// libs
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



        vkInstance::vkInstance() :
            m_Instance(VK_NULL_HANDLE),
        #ifdef OC_DEBUG
            m_DebugCallback(VK_NULL_HANDLE),
        #endif
            m_Devices(1)
        {
            oprint("Initializing Vulkan Instance!\n");

            i32 gladVersion = gladLoaderLoadVulkan(nullptr, nullptr, nullptr);
            if (!gladVersion)
                throw Exception(Error::SYSTEM_ERROR, "Unable to load Vulkan symbols! gladLoad Failure.");

            u32 instanceExtensionCount;
            vkCheck(
                vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, nullptr)
            );

            DynamicArray<VkExtensionProperties> availableExtensions(instanceExtensionCount);
            vkCheck(
                vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, availableExtensions.data())
            );

            DynamicArray<cstring> requiredExtensions { VK_KHR_SURFACE_EXTENSION_NAME };

        #ifdef OC_DEBUG

            b8 hasDebugCapabilities = false;
            for (const VkExtensionProperties& extension : availableExtensions) {
                if (std::strncmp(extension.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME, strlen(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)) == 0) {
                    hasDebugCapabilities = true;

                    break;
                }
            }

            if (hasDebugCapabilities)
                requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            else
                oprint(CONSOLE_TEXT_YELLOW("%s is not available...Disabling debug utils messenger."), VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        #endif

            

            u32 instanceLayerCount;
            vkCheck(
                vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr)
            );

            
        }

        vkInstance::~vkInstance() {
            
        }

    }   // Splash

}   // Ocean
