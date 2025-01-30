#pragma once

/**
 * @file vk_RendererAPI.hpp
 * @author Evan F.
 * @brief 
 * @date 2025-01-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Types/Strings.hpp"
#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Array.hpp"

#include "Ocean/Renderer/RendererAPI.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {
    
        class vkRendererAPI : public RendererAPI {
        public:
            virtual void Init() override final;

            virtual void SetViewport(u32 x, u32 y, u32 w, u32 h) override final;

            virtual void SetClearColor(const glm::vec4& color) override final;
            virtual void Clear() override final;

            virtual void DrawIndexed(const Ref<VertexArray>& array, u32 indexCount) override final;

        private:
            VkInstance m_Instance;

            VkPhysicalDevice m_gpu;
            // VkPhysicalDeviceProperties m_gpuProperties;
            // VkPhysicalDeviceFeatures m_gpuFeatures;

            VkDevice m_Device;
            DynamicArray<cstring> m_Extensions;
            DynamicArray<cstring> m_Layers;

            VkQueue m_Queue;
            DynamicArray<VkQueueFamilyProperties> m_QueueProperties;
            u32 m_GraphicsQueueIndex;

            VkCommandPool m_CommandPool;

        };  // RendererAPI

    }   // Splash

}   // Ocean
