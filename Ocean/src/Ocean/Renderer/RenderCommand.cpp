#include "ocpch.hpp"

#include "Ocean/Renderer/RenderCommand.hpp"

namespace Ocean {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}
