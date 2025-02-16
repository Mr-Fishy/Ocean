#pragma once

/**
 * @file vk_Pipeline.hpp
 * @author Evan F.
 * @brief 
 * 
 * @copyright Copyright (c) 2025
 * 
 */
 
 #include "Ocean/Types/Strings.hpp"
 
 #include "Ocean/Primitives/HashMap.hpp"
 #include "Ocean/Primitives/Macros.hpp"
 
 #include "Ocean/Renderer/Vulkan/vk_RenderPass.hpp"
 #include "Ocean/Renderer/Vulkan/vk_Shader.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {
    
        class vkPipeline {
        public:
            vkPipeline();
            ~vkPipeline();

            OC_INLINE void AddRenderPass(cstring name, const vkRenderPass& renderPass) { this->m_RenderPasses[name] = renderPass; Invalidate(); }
            OC_INLINE void RemoveRenderPass(cstring name) { this->m_RenderPasses.erase(name); Invalidate(); }

            void Invalidate();

            OC_INLINE Ref<vkShader> Shader() const { return m_Shader; }
            OC_INLINE void SetShader(const Ref<vkShader> &Shader) { m_Shader = Shader; }

        private:
            VkPipeline m_Pipeline;
            VkPipelineLayout m_Layout;

            /** @todo From my understanding (and according to the code), only one renderpass can be assigned to a pipeline. And multiple render passes / pipelines are only needed when you wish to access image data from another rendered image. */
            UnorderedMap<cstring, vkRenderPass> m_RenderPasses;

            Ref<vkShader> m_Shader;

        };  // vkPipeline

    }   // Splash

}   // Ocean
