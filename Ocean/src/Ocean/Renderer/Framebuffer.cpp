#include "ocpch.hpp"

#include "Ocean/Renderer/Framebuffer.hpp"

#include "Ocean/Renderer/Renderer.hpp"
#include "Platform/OpenGL/OpenGLFramebuffer.hpp"
#include "Platform/Vk/VulkanFramebuffer.hpp"

namespace Ocean {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpec& spec) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateRef<GL::OpenGLFramebuffer>(spec);

			case RendererAPI::API::Vulkan:
				return CreateRef<Vk::VulkanFramebuffer>(spec);
		}

		return nullptr;
	}

}	// Ocean
