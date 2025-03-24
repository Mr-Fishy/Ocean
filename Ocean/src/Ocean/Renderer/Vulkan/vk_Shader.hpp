#pragma once

/**
 * @file vk_Shader.hpp
 * @author Evan F.
 * @brief 
 * @date 2025-01-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Renderer/Shader.hpp"
#include "Ocean/Types/Strings.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {
    
        class vkShader : public Shader {
        public:
            vkShader(const cstring vertexSource, const cstring fragmentSource, const cstring geometrySource = nullptr);
            virtual ~vkShader();

            virtual void Bind() const override final;
            virtual void Unbind() const override final;

            virtual void SetInt(cstring name, i32 value) const override final;
            virtual void SetIntArray(cstring name, i32* array, u32 length) const override final;

            virtual void SetFloat(cstring name, f32 value) const override final;
            virtual void SetVec2f(cstring name, const glm::vec2& value) const override final;
            virtual void SetVec3f(cstring name, const glm::vec3& value) const override final;
            virtual void SetVec4f(cstring name, const glm::vec4& value) const override final;

            virtual void SetMat4f(cstring name, const glm::mat4& value) const override final;
            virtual void SetMat3f(cstring name, const glm::mat3& value) const override final;

            VkShaderModule Vert() const { return m_Vert; }
            VkShaderModule Frag() const { return m_Frag; }

        private:
            VkShaderModule m_Vert;
            VkShaderModule m_Frag;

        };

    }   // Ocean

}   // Ocean
