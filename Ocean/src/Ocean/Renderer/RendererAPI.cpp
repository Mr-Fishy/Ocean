
#include "ocpch.hpp"
#include "RendererAPI.hpp"

#include "Platform/OpenGL/OpenGLRendererAPI.hpp"

namespace Ocean {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create() {
		switch (s_API) {
			case RendererAPI::API::None:
				OC_CORE_ASSERT(false, "RendererAPI::None is currently not supportedd!")
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateScope<OpenGLRendererAPI>();
		}

		OC_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}

}	// Ocean
