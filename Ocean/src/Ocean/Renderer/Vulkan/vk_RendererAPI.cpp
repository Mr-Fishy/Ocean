#include "vk_RendererAPI.hpp"

#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Renderer/RendererAPI.hpp"
#include "Ocean/Renderer/VertexArray.hpp"

#include "Ocean/Renderer/Vulkan/vk_Vulkan.hpp"
#include "Ocean/Renderer/Vulkan/vk_Instance.hpp"

// std
#include <cstring>

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {

        // https://github.com/Dav1dde/glad/blob/glad2/example/c/vulkan_tri_glfw/vulkan_tri_glfw.c#L2082

        // https://github.com/KhronosGroup/Vulkan-Samples/blob/main/samples/api/hello_triangle_1_3/hello_triangle_1_3.h
        // https://github.com/KhronosGroup/Vulkan-Samples/blob/main/samples/api/hello_triangle_1_3/hello_triangle_1_3.cpp

        vkRendererAPI::vkRendererAPI() : RendererAPI() {
            vkInstance::Get();
        }

        vkRendererAPI::~vkRendererAPI() {

        }

        void vkRendererAPI::Init() {
            const VkCommandPoolCreateInfo poolInfo {
                VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                nullptr,
                VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                vkInstance::Get().Swapchain()->GraphicsQueueIndex()
            };

            vkCheck(
                vkCreateCommandPool(vkInstance::Get().Device()->GetLogical(), &poolInfo, nullptr, &this->m_CommandPool)
            );

            const VkCommandBufferAllocateInfo cmdInfo {
                VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                nullptr,
                this->m_CommandPool,
                VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                1
            };

            vkCheck(
                vkAllocateCommandBuffers(vkInstance::Get().Device()->GetLogical(), &cmdInfo, &this->m_DrawBuffer)
            );
        }

        void vkRendererAPI::SetViewport(u32 x, u32 y, u32 w, u32 h) {
            
        }

        void vkRendererAPI::SetClearColor(const glm::vec4& color) {

        }

        void vkRendererAPI::Clear() {

        }

        void vkRendererAPI::DrawIndexed(const Ref<VertexArray>& array, u32 indexCount) {
            // u32 count = indexCount ? indexCount : array->GetIndexBuffer()->GetCount();

        }

    }   // Splash

}   // Ocean
