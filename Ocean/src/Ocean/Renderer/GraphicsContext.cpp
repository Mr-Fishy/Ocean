
#include "ocpch.hpp"

#include "Ocean/Renderer/GraphicsContext.hpp"

#include "Ocean/Renderer/Renderer.hpp"
#include "Platform/OpenGL/OpenGLContext.hpp"

namespace Ocean {

	Scope<GraphicsContext> GraphicsContext::Create(void* window) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				OC_CORE_ASSERT(false, "RendererAPI::None is currently not supportedd!")
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		OC_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}

}	// Ocean