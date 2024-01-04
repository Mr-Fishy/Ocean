
#include "ocpch.hpp"
#include "Framebuffer.hpp"

#include "Ocean/Renderer/Renderer.hpp"

#include "Platform/OpenGL/OpenGLFramebuffer.hpp"

namespace Ocean {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				OC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLFramebuffer>(spec);
		}

		OC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}	// Ocean