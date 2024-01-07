#pragma once

#include <glm/glm.hpp>

#include "Ocean/Renderer/VertexArray.hpp"

namespace Ocean {

	// The API that is being used.
	// There are also basic functions for general renderer use.
	//
	class RendererAPI {
	public:
		enum class API {
			None = 0,
			OpenGL = 1,
			Vulkan = 2,
		};

		/* --- */

		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		// View RenderCommand for description.
		//
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		// View RenderCommand for description.
		//
		virtual void SetClearColor(const glm::vec4& color) = 0;
		// View RenderCommand for description.
		//
		virtual void Clear() = 0;

		// View RenderCommand for description.
		//
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

		static API GetAPI() { return s_API; }
		static Scope<RendererAPI> Create();

	private:
		static API s_API;
	};

}	// Ocean