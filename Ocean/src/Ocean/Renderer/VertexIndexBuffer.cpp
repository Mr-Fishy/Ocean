#include "ocpch.hpp"

#include "Ocean/Renderer/VertexIndexBuffer.hpp"

#include "Ocean/Renderer/Renderer.hpp"
#include "Platform/OpenGL/OpenGLVertexIndexBuffer.hpp"
#include "Platform/Vk/VulkanVertexIndexBuffer.hpp"

namespace Ocean {

	Ref<VertexBuffer> VertexBuffer::Create(ui32 size) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				// OC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateRef<GL::OpenGLVertexBuffer>(size);

			case RendererAPI::API::Vulkan:
				return CreateRef<Vk::VulkanVertexBuffer>(size);
		}

		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, ui32 size) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				// OC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateRef<GL::OpenGLVertexBuffer>(vertices, size);

			case RendererAPI::API::Vulkan:
				return CreateRef<Vk::VulkanVertexBuffer>(vertices, size);
		}

		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(ui32* indices, ui32 count) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				// OC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateRef<GL::OpenGLIndexBuffer>(indices, count);

			case RendererAPI::API::Vulkan:
				return CreateRef<Vk::VulkanIndexBuffer>(indices, count);
		}

		return nullptr;
	}

}	// Ocean
