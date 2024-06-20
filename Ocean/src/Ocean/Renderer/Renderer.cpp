#include "ocpch.hpp"

#include "Ocean/Renderer/Renderer.hpp"

#include "Ocean/Renderer/Renderer2D.hpp"

namespace Ocean {

	Scope< Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init() {
		RenderCommand::Init();

		Renderer2D::Init();
	}

	void Renderer::Shutdown() {
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(ui32 width, ui32 height) {
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera) {
		s_SceneData->ViewProjectionMatrix = camera.GetProjectionViewMatrix();
	}

	void Renderer::EndScene() { }

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const mat4 transform) {
		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();

		RenderCommand::DrawIndexed(vertexArray);
	}

}	// Ocean
