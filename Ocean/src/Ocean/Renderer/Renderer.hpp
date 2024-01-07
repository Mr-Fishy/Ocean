#pragma once

#include "Ocean/Renderer/RenderCommand.hpp"

#include "Ocean/Renderer/Camera/OrthoCamera.hpp"
#include "Ocean/Renderer/Shader.hpp"

namespace Ocean {

	// The base-class renderer, this renderer does not have a specified dimension.
	//
	class Renderer {
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		// Begins the rendering scene for the layer and completes pre-rendering checks / calculations.
		// @param camera: The orthographic camera to use when projecting objects in the scene view.
		//
		static void BeginScene(OrthographicCamera& camera);
		// Ends the rendering scene for the layer.
		//
		static void EndScene();

		// Submits the VertexArray into the render queue.
		// @param shader: The shader for rendering.
		// @param vertexArray: The VertexArray of elements to be rendered.
		// @param transform: The current camera transform for the shader.
		//
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> s_SceneData;
	};

}	// Ocean
