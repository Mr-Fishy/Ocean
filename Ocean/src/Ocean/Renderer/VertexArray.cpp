#include "ocpch.hpp"

#include "Ocean/Renderer/VertexArray.hpp"

#include "Ocean/Renderer/Renderer.hpp"
#include "Platform/OpenGL/OpenGLVertexArray.hpp"
#include "Platform/Vk/VulkanVertexArray.hpp"

namespace Ocean {

	Ref<VertexArray> VertexArray::Create() {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateRef<GL::OpenGLVertexArray>();

			case RendererAPI::API::Vulkan:
				return CreateRef<Vk::VulkanVertexArray>();
		}

		return nullptr;
	}

}	// Ocean
