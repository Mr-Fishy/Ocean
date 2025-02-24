#include "vk_Shader.hpp"

#include "Ocean/Types/Integers.hpp"

#include "Ocean/Primitives/Array.hpp"

#include "Ocean/Renderer/Vulkan/vk_Vulkan.hpp"
#include "Ocean/Renderer/Vulkan/vk_Instance.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {

        vkShader::vkShader(const cstring vertexSource, const cstring fragmentSource, const cstring geometrySource) :
            m_Vert(VK_NULL_HANDLE),
            m_Frag(VK_NULL_HANDLE)
        {
            // ============================== COMPILE TO SPIRV ==============================
            //
            // Take the given source code and send it to the compiler to get the SPIRV equivalent.
            //
            DynamicArray<u32> vertex = Compiler::CompileToSpirv(vertexSource, Compiler::ShaderStage::VERTEX_SHADER);

            DynamicArray<u32> fragment = Compiler::CompileToSpirv(fragmentSource, Compiler::ShaderStage::FRAGMENT_SHADER);

            // ============================== VERTEX SHADER ==============================
            //
            // Take's the vertex shader SPIRV code and make it into a Vulkan Shader Module.
            // After this point the SPIRV code is not needed for runtime.
            //
            // In order to reduce the need to compiler shader's at every startup they can be cached.
            //
            VkShaderModuleCreateInfo vertInfo {
                VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
                nullptr,
                0,
                vertex.size() * sizeof(u32),
                vertex.data()
            };

            vkCheck(
                vkCreateShaderModule(vkInstance::Get().Device()->Logical(), &vertInfo, nullptr, &this->m_Vert)
            );

            // ============================== FRAGMENT SHADER ==============================
            //
            // Take's the fragment shader SPIRV code and make it into a Vulkan Shader Module.
            // After this point the SPIRV code is not needed for runtime.
            //
            VkShaderModuleCreateInfo fragInfo {
                VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
                nullptr,
                0,
                fragment.size() * sizeof(u32),
                fragment.data()
            };

            vkCheck(
                vkCreateShaderModule(vkInstance::Get().Device()->Logical(), &fragInfo, nullptr, &this->m_Frag)
            );
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
