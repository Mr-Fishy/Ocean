#include "Renderer.hpp"

#include "Ocean/Scene/Camera.hpp"

#include "Renderer/RenderCommand.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/VertexArray.hpp"
#include "Renderer/Renderer2D.hpp"

namespace Ocean {

    namespace Shrimp {

        void Renderer::Init() {
            RenderCommand::Init();
            
            // Renderer2D::Init();
        }

        void Renderer::Shutdown() {
            // Renderer2D::Shutdown();
        }

        void Renderer::BeginScene(Camera& camera) {
            s_SceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
        }

        void Renderer::EndScene() {

        }

        void Renderer::Submit(const SharedPtr<Shader>& shader, const SharedPtr<VertexArray>& array, glm::mat4 transform) {
            shader->Bind();
            shader->SetMat4f("u_ViewProjection", s_SceneData->viewProjectionMatrix);
            shader->SetMat4f("u_Transform", transform);

            array->Bind();
            RenderCommand::DrawIndexed(array);
        }

        void Renderer::OnWindowResize(u32 width, u32 height) {
            RenderCommand::SetViewport(0, 0, width, height);
        }

    }   // Shrimp

}   // Ocean
