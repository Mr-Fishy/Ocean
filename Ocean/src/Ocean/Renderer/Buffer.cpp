
#include "ocpch.hpp"
#include "Buffer.hpp"

#include "Renderer.hpp"

#include "Platform/OpenGL/OpenGLBuffer.hpp"

namespace Ocean {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				OC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return new OpenGLVertexBuffer(vertices, size);

			default:
				OC_CORE_ASSERT(false, "Unkown RendererAPI!");
				return nullptr;
		}
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size) {
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				OC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return new OpenGLIndexBuffer(indices, size);

			default:
				OC_CORE_ASSERT(false, "Unkown RendererAPI!");
				return nullptr;
		}
	}

}	// Ocean