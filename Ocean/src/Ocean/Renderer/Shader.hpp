#pragma once

/**
 * @file Shader.hpp
 * @author Evan F.
 * @brief The abstract Shader for Ocean.
 * @date 01-14-2025
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Primitives/Array.hpp"
#include "Ocean/Primitives/Macros.hpp"
#include "Ocean/Types/FloatingPoints.hpp"
#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/SmartPtrs.hpp"
#include "Ocean/Types/Strings.hpp"

// libs
#include <glm/glm.hpp>

namespace Ocean {

    namespace Splash {
    
        /**
         * @brief The Shader controls a shader through the renderer API.
         */
        class Shader {
        private:
            /** @todo Runtime shader compilation using shaderc and spirv-cross or valid alternative. */
            /**
             * @brief The Compiler will enable runtime compilation and caching of shader files for the current API.
             */
            class Compiler {
                OC_STATIC DynamicArray<u32> CompileToSpirv(const DynamicArray<u8>& source);

            };  // Compiler

        public:
            virtual ~Shader() = default;

            /**
             * @brief Bind's the Shader to be used by commmands.
             */
            virtual void Bind() const = 0;
            /**
             * @brief Unbind's the Shader from being able to be used by commands.
             */
            virtual void Unbind() const = 0;

            /**
             * @brief Set the integer value to the element with the given name.
             * 
             * @param name The name of the element.
             * @param value The new value.
             */
            virtual void SetInt(cstring name, i32 value) const = 0;
            /**
             * @brief Set the integer array to the element with the given name.
             * 
             * @param name The name of the element.
             * @param array The array of new values.
             * @param length The length of the array.
             */
            virtual void SetIntArray(cstring name, i32* array, u32 length) const = 0;

            /**
             * @brief Set the float value to the element with the given name.
             * 
             * @param name The name of the element.
             * @param value The new value.
             */
            virtual void SetFloat(cstring name, f32 value) const = 0;
            /**
             * @brief Set the vec2 value to the element with the given name.
             * 
             * @param name The name of the element.
             * @param value The new value.
             */
            virtual void SetVec2f(cstring name, const glm::vec2& value) const = 0;
            /**
             * @brief Set the vec3 value to the element with the given name.
             * 
             * @param name The name of the element.
             * @param value The new value.
             */
            virtual void SetVec3f(cstring name, const glm::vec3& value) const = 0;
            /**
             * @brief Set the vec4 value to the element with the given name.
             * 
             * @param name The name of the element.
             * @param value The new value.
             */
            virtual void SetVec4f(cstring name, const glm::vec4& value) const = 0;

            /**
             * @brief Set the 4x4 matrix value to the element with the given name.
             * 
             * @param name The name of the element.
             * @param value The new value.
             */
            virtual void SetMat4f(cstring name, const glm::mat4& value) const = 0;
            /**
             * @brief Set the 3x3 matrix value to the element with the given name.
             * 
             * @param name The name of the element.
             * @param value The new value.
             */
            virtual void SetMat3f(cstring name, const glm::mat3& value) const = 0;

            /**
             * @brief Create's a new Shader with the given sources.
             * 
             * @param vertexSource The source of the vertex shader.
             * @param fragmentSource The source of the fragment shader.
             * @param geometrySource The source of the geometry shader. (OPTIONAL)
             * @return Ref<Shader> 
             */
            // OC_STATIC Ref<Shader> Create(cstring vertexSource, cstring fragmentSource, cstring geometrySource = nullptr);
            OC_STATIC Ref<Shader> Create(const DynamicArray<i8>& vertexSource, const DynamicArray<i8>& fragmentSource, const DynamicArray<i8>& geometrySource = { });

        };  // Shader

    }   // Ocean

}   // Ocean
