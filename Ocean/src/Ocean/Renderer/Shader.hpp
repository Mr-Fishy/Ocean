#pragma once

#include "Ocean/Types/FloatingPoints.hpp"
#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/SmartPtrs.hpp"
#include "Ocean/Types/Strings.hpp"

// libs
#include <glm/glm.hpp>

namespace Ocean {

    namespace Shrimp {
    
        class Shader {
        public:
            virtual ~Shader() = default;

            virtual void Bind() const = 0;
            virtual void Unbind() const = 0;

            virtual void SetInt(cstring name, i32 value) const = 0;
            virtual void SetIntArray(cstring name, i32* array, u32 length) const = 0;

            virtual void SetFloat(cstring name, f32 value) const = 0;
            virtual void SetVec2f(cstring name, const glm::vec2& value) const = 0;
            virtual void SetVec3f(cstring name, const glm::vec3& value) const = 0;
            virtual void SetVec4f(cstring name, const glm::vec4& value) const = 0;

            virtual void SetMat4f(cstring name, const glm::mat4& value) const = 0;
            virtual void SetMat3f(cstring name, const glm::mat3& value) const = 0;

            static Ref<Shader> Create(cstring vertexSource, cstring fragmentSource, cstring geometrySource = nullptr);

        };  // Shader

    }   // Ocean

}   // Ocean
