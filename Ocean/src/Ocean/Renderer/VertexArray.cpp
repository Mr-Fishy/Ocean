
#include "ocpch.hpp"
#include "VertexArray.hpp"

#include "Ocean/Renderer/Renderer.hpp"
#include "Platform/OpenGL/OpenGLVertexArray.hpp"

namespace Ocean {

	Ref<VertexArray> VertexArray::Create() {
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				OC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLVertexArray>();
		}

		OC_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}

}	// Ocean