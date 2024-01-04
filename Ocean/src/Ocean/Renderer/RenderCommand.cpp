
#include "ocpch.hpp"
#include "RenderCommand.hpp"

#include "Platform/OpenGL/OpenGLRendererAPI.hpp"

namespace Ocean {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}	// Ocean
