#include "vk_Shader.hpp"

namespace Ocean {

    namespace Splash {

        vkShader::vkShader(cstring vertexSource, cstring fragmentSource, cstring geometrySource) : m_Shader() {
            
        }

        vkShader::~vkShader() {
            
        }

        void vkShader::Bind() const {
            
        }

        void vkShader::Unbind() const {
            
        }

        void vkShader::SetInt(cstring name, i32 value) const {
            
        }

        void vkShader::SetIntArray(cstring name, i32* array, u32 length) const {
            
        }

        void vkShader::SetFloat(cstring name, f32 value) const {
            
        }

        void vkShader::SetVec2f(cstring name, const glm::vec2& value) const {
            
        }

        void vkShader::SetVec3f(cstring name, const glm::vec3& value) const {
            
        }

        void vkShader::SetVec4f(cstring name, const glm::vec4& value) const {
            
        }

        void vkShader::SetMat4f(cstring name, const glm::mat4& value) const {
            
        }

        void vkShader::SetMat3f(cstring name, const glm::mat3& value) const {
            
        }

    }   // Splash

}   // Ocean
