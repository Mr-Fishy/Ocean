#pragma once

/**
 * @file vk_RendererAPI.hpp
 * @author Evan F.
 * @brief 
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Types/Bool.hpp"
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
            vkRendererAPI();
            ~vkRendererAPI();

            virtual void Init() override final;

            virtual void SetViewport(u32 x, u32 y, u32 w, u32 h) override final;

            virtual void SetClearColor(const glm::vec4& color) override final;
            virtual void Clear() override final;

            virtual void DrawIndexed(const Ref<VertexArray>& array, u32 indexCount) override final;

            VkInstance GetInstance() const { return this->m_Instance; }

            VkPhysicalDevice GetPhysicalDevice() const { return this->m_gpu; }
            VkDevice GetDevice() const { return this->m_Device; }

            VkQueue* GetQueue() { return &this->m_Queue; }

            DynamicArray<VkQueueFamilyProperties>& GetQueueProperties() { return this->m_QueueProperties; }
            void SetGraphicsQueueIndex(u32 index) { this->m_GraphicsQueueIndex = index; }
            void SetPresentQueueIndex(u32 index) { this->m_PresentQueueIndex = index; }

            void InitDevice();

        private:
            b32 ValidateLayers();

            /* --- */

            VkInstance m_Instance;

        #ifdef OC_DEBUG

            PFN_vkCreateDebugReportCallbackEXT m_CreateDebugCallback;
            PFN_vkDestroyDebugReportCallbackEXT m_DestroyDebugCallback;
            PFN_vkDebugReportMessageEXT m_MessageCallback;

        #endif

            DynamicArray<cstring> m_Extensions;
            DynamicArray<cstring> m_Layers;

            VkQueue m_Queue;
            DynamicArray<VkQueueFamilyProperties> m_QueueProperties;
            u32 m_GraphicsQueueIndex;
            u32 m_PresentQueueIndex;

            VkCommandPool m_CommandPool;

        };  // RendererAPI

    }   // Splash

}   // Ocean
