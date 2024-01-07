#pragma once

#include "Ocean/Renderer/RendererAPI.hpp"

namespace Ocean {

	// Basic platform independent render commands for runtime.
	//
	class RenderCommand {
	public:
		static void Init() {
			s_RendererAPI->Init();
		}

		// Sets the window viewport (as a rectangle).
		// @param x, y: Specifies the bottom left corner of the viewport.
		// @param width, height: Specifies the width and height of the viewport.
		//
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		// Sets the clear color of the viewport (background color).
		// @param color: A glm::vec4 color.
		//
		static void SetClearColor(const glm::vec4& color) {
			s_RendererAPI->SetClearColor(color);
		}

		// Clears the currently held buffers. In this case GL_COLOR_BUFFER_BIT & GL_DEPTH_BUFFER_BIT.
		//
		static void Clear() {
			s_RendererAPI->Clear();
		}

		// Specifies the number of elements (objects) to render.
		// @param vertexArray: The VertexArray to render (in triangles).
		// @param count: The number of elements to render.
		//
		static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0) {
			s_RendererAPI->DrawIndexed(vertexArray, count);
		}

	private:
		static Scope<RendererAPI> s_RendererAPI;
	};

}	// Ocean
