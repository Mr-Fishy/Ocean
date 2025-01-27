#include "Renderer.hpp"

#include "Ocean/Renderer/RenderCommand.hpp"
#include "Ocean/Renderer/Shader.hpp"
#include "Ocean/Renderer/VertexArray.hpp"
#include "Ocean/Renderer/Renderer2D.hpp"

#include "Ocean/Renderer/Camera/Camera.hpp"

namespace Ocean {

    void Renderer::Init() {
        RenderCommand::Init();
        
        Renderer2D::Init();
    }

    void Renderer::Shutdown() {
        Renderer2D::Shutdown();
    }

    void Renderer::BeginScene(Camera& camera) {
        s_SceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene() {

    }

    void Renderer::Submit(const Ref<Splash::Shader>& shader, const Ref<Splash::VertexArray>& array, glm::mat4 transform) {
        shader->Bind();
        shader->SetMat4f("u_ViewProjection", s_SceneData->viewProjectionMatrix);
        shader->SetMat4f("u_Transform", transform);

        array->Bind();
        RenderCommand::DrawIndexed(array);
    }

    void Renderer::OnWindowResize(u32 width, u32 height) {
        RenderCommand::SetViewport(0, 0, width, height);
    }

}   // Ocean
