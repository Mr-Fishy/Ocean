#include "ocpch.hpp"

#include "Ocean/Renderer/RendererAPI.hpp"

#include "Platform/OpenGL/OpenGLRendererAPI.hpp"
#include "Platform/Vk/VulkanRendererAPI.hpp"

namespace Ocean {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create() {
		switch (s_API) {
			case RendererAPI::API::None:
				OC_CORE_ERROR("RendererAPI::None is not a valid API!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateScope<GL::OpenGLRendererAPI>();

			case RendererAPI::API::Vulkan:
				return CreateScope<Vk::VulkanRendererAPI>();
		}

		OC_CORE_ERROR("Unknown RendererAPI!");
		return nullptr;
	}

}	// Ocean
