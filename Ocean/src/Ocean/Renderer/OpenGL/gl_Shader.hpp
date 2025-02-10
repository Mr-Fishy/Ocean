#pragma once

/**
 * @file gl_Shader.hpp
 * @author Evan F.
 * @brief 
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Renderer/Shader.hpp"

namespace Ocean {

    namespace Splash {
    
        class glShader : public Shader {
        private:
            /** @brief The type of compile operation for either a shader program or a shader. */
            enum CompileType {
                PROGRAM = 0,
                SHADER  = 1,
            };

        public:
            glShader(const DynamicArray<i8>& vertexSource, const DynamicArray<i8>& fragmentSource, const DynamicArray<i8>& geometrySource = { });
            virtual ~glShader();

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

        private:
            /**
             * @brief Checks for compilation erros from OpenGL.
             * 
             * @param object The object to check on.
             * @param type The type of compilation operation the object is from.
             */
            void CheckCompileErrors(u32 object, CompileType type);

            u32 m_RendererID;

        };

    }   // Ocean

}   // Ocean
