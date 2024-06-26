#pragma once

#include "Ocean/Renderer/RenderCommand.hpp"

#include "Ocean/Renderer/Camera.hpp"
#include "Ocean/Renderer/Shader.hpp"

namespace Ocean {

	class Renderer {
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(ui32 width, ui32 height);

		static void BeginScene(OrthographicCamera& camera);
		// static void BeginScene(PerspectiveCamera& camera);

		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const mat4 transform = mat4(1.0f));

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData {
			mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> s_SceneData;
	};

}	// Ocean
