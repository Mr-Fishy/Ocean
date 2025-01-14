#pragma once

#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/SmartPtrs.hpp"

// libs
#include <glm/glm.hpp>

namespace Ocean {

    class Camera;

    namespace Shrimp {

        class VertexArray;
        class Shader;

    }   // Shrimp

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

        static void Submit(const Ref<Shrimp::Shader>& shader, const Ref<Shrimp::VertexArray>& array, glm::mat4 transform = glm::mat4(1.0f));

        static void OnWindowResize(u32 width, u32 height);

    private:
        static inline Scope<SceneData> s_SceneData;

    };  // Renderer

}   // Ocean
