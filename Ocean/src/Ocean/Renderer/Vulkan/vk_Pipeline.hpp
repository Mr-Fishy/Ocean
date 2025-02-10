#pragma once

/**
 * @file vk_Pipeline.hpp
 * @author Evan F.
 * @brief 
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Primitives/HashMap.hpp"
#include "Ocean/Primitives/Macros.hpp"
#include "Ocean/Types/Strings.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {

        class vkRenderPass {
        public:
            vkRenderPass();
            ~vkRenderPass();

        };  // vkRenderPass
    
        class vkPipeline {
        public:
            vkPipeline();
            ~vkPipeline();

            OC_INLINE void AddRenderPass(cstring name, const vkRenderPass& renderPass) { this->m_RenderPasses[name] = renderPass; Invalidate(); }
            OC_INLINE void RemoveRenderPass(cstring name) { this->m_RenderPasses.erase(name); Invalidate(); }

            void Invalidate();

        private:
            VkPipeline m_Pipeline;
            VkPipelineLayout m_Layout;

            UnorderedMap<cstring, vkRenderPass> m_RenderPasses;

        };  // vkPipeline

    }   // Splash

}   // Ocean
