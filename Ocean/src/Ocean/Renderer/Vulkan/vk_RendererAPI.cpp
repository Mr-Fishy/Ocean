#include "vk_RendererAPI.hpp"

#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Macros.hpp"

#include "Ocean/Renderer/VertexArray.hpp"

// libs
#define GLAD_VULKAN_IMPLEMENTATION
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {

        OC_STATIC VKAPI_ATTR b32 VKAPI_CALL vkMessageCallback(
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

    // https://github.com/Dav1dde/glad/blob/glad2/example/c/vulkan_tri_glfw/vulkan_tri_glfw.c#L2082

    // https://github.com/KhronosGroup/Vulkan-Samples/blob/main/samples/api/hello_triangle_1_3/hello_triangle_1_3.h
    // https://github.com/KhronosGroup/Vulkan-Samples/blob/main/samples/api/hello_triangle_1_3/hello_triangle_1_3.cpp

        void vkRendererAPI::Init() {
            int vulkanVersion = gladloaderLoadVulkan(NULL, NULL, NULL);



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

    }   // Splash

}   // Ocean
