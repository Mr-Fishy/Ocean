#pragma once

#include "Ocean/Renderer/RendererAPI.hpp"

namespace Ocean {

	class RenderCommand {
	public:
		static void Init() {
			s_RendererAPI->Init();
		}

		static void SetViewport(ui32 x, ui32 y, ui32 width, ui32 height) {
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		static void SetClearColor(const vec4& color) {
			s_RendererAPI->SetClearColor(color);
		}

		static void Clear() {
			s_RendererAPI->Clear();
		}

		static void DrawIndexed(const Ref<VertexArray>& vertexArray, ui32 count = 0) {
			s_RendererAPI->DrawIndexed(vertexArray, count);
		}

	private:
		static Scope<RendererAPI> s_RendererAPI;
	};

}	// Ocean
