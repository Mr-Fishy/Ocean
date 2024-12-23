#pragma once

#include "Ocean/Core/Types/SharedPtr.hpp"
#include "Ocean/Core/Types/UniquePtr.hpp"

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

            static void Submit(const SharedPtr<Shader>& shader, const SharedPtr<VertexArray>& array, glm::mat4 transform = glm::mat4(1.0f));

            static void OnWindowResize(u32 width, u32 height);

        private:
            static inline UniquePtr<SceneData> s_SceneData = MakeUniquePtr<SceneData>();

        };  // Renderer

    }   // Shrimp

}   // Ocean
