#pragma once

#include "Ocean/Core/Types/FloatingPoints.hpp"
#include "Ocean/Core/Types/Integers.hpp"
#include "Ocean/Core/Types/SmartPtrs.hpp"

#include "Renderer/Texture.hpp"

// libs
#include <glm/glm.hpp>

namespace Ocean {

    class Camera;

    namespace Shrimp {

        class Renderer2D {
        public:
            static void Init();
            static void Shutdown();

            static void BeginScene(const Camera& camera);
            // TODO: Editor Camera
            static void EndScene();
            static void Flush();

            static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
            static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color);

            static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Texture2D>& texture, f32 tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
            static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Texture2D>& texture, f32 tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

            static void DrawQuad(const glm::mat4& transform, const glm::vec4 color, i32 entityID = -1);
            static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, f32 tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), i32 entityID = -1);
            
            struct Statistics {
                u32 drawCalls = 0;
                u32 quadCount = 0;

                inline u32 GetVertexCount() const { return this->quadCount * 4; }
                inline u32 GetIndexCount() const { return this->quadCount * 6; }

            };  // Statistics

            static void ResetStats();
            static Statistics GetStats();

        private:
            static void StartBatch();
            static void NextBatch();

        };  // Renderer2D

    }   // Shrimp

}   // Ocean
