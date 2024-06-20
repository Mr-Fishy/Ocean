#pragma once

#include "Ocean/Core/Base.hpp"
#include "Ocean/Core/Types.hpp"

#include "Ocean/Renderer/VertexArray.hpp"

namespace Ocean {

	class RendererAPI {
	public:
		enum class API {
			None   = 0,
			OpenGL = 1,
			Vulkan = 2,
		};

		virtual ~RendererAPI() = default;

		virtual void Init() = 0;

		virtual void SetViewport(ui32 x, ui32 y, ui32 width, ui32 height) = 0;

		virtual void SetClearColor(const vec4& color) = 0;

		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, ui32 indexCount = 0) = 0;

		static Scope<RendererAPI> Create();
		static API GetAPI() { return s_API; }

	private:
		static API s_API;
	};

}
