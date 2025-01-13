#pragma once

#include "Ocean/Core/Types/Integers.hpp"
#include "Ocean/Core/Types/SmartPtrs.hpp"

// libs
#include <glm/glm.hpp>

namespace Ocean {

    class Camera;

    namespace Shrimp {

        class VertexArray;
        class Shader;

        class Renderer {
        private:
            struct SceneData {
                glm::mat4 viewProjectionMatrix;

            };  // SceneData

        public:
            static void Init();
            static void Shutdown();

            static void BeginScene(Camera& camera);
            static void EndScene();

            static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& array, glm::mat4 transform = glm::mat4(1.0f));

            static void OnWindowResize(u32 width, u32 height);

        private:
            static inline Scope<SceneData> s_SceneData;

        };  // Renderer

    }   // Shrimp

}   // Ocean
