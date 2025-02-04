#pragma once

/**
 * @file vk_RendererAPI.hpp
 * @author Evan F.
 * @brief 
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Primitives/Macros.hpp"
#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Renderer/RendererAPI.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {
    
        class vkRendererAPI : public RendererAPI {
        public:
            vkRendererAPI();
            virtual ~vkRendererAPI();

            virtual void Init() override final;

            virtual void SetViewport(u32 x, u32 y, u32 w, u32 h) override final;

            virtual void SetClearColor(const glm::vec4& color) override final;
            virtual void Clear() override final;

            virtual void DrawIndexed(const Ref<VertexArray>& array, u32 indexCount) override final;

        private:
            OC_NO_COPY(vkRendererAPI);

            VkCommandPool m_CommandPool;

            VkCommandBuffer m_DrawBuffer;

        };  // RendererAPI

    }   // Splash

}   // Ocean
