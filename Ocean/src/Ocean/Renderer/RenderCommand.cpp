
#include "ocpch.hpp"
#include "RenderCommand.hpp"

#include "Platform/OpenGL/OpenGLRendererAPI.hpp"

namespace Ocean {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}	// Ocean
