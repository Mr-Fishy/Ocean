
#include "ocpch.hpp"
#include "Buffer.hpp"

#include "Ocean/Renderer/Renderer.hpp"

#include "Platform/OpenGL/OpenGLBuffer.hpp"

namespace Ocean {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				OC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLVertexBuffer>(size);
		}

		OC_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				OC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		OC_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				OC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLIndexBuffer>(indices, count);
		}

		OC_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}

}	// Ocean