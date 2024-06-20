#include "ocpch.hpp"

#include "Ocean/Renderer/GraphicsContext.hpp"

#include "Ocean/Core/Base.hpp"

#include "Ocean/Renderer/Renderer.hpp"
#include "Platform/OpenGL/OpenGLContext.hpp"
#include "Platform/Vk/VulkanContext.hpp"

namespace Ocean {

	Scope<GraphicsContext> GraphicsContext::Create(void* window) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateScope<GL::OpenGLContext>(static_cast<GLFWwindow*>(window));

			case RendererAPI::API::Vulkan:
				return CreateScope<Vk::VulkanContext>(static_cast<GLFWwindow*>(window));
		}

		return nullptr;
	}

}	// Ocean
