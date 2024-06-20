#include "ocpch.hpp"

#include "Platform/OpenGL/OpenGLRendererAPI.hpp"

#include "Ocean/Core/Base.hpp"

// libs
#include <glad/gl.h>

namespace Ocean::GL {

	static void OpenGLMessageCallback(
		ui32 source, ui32 type, ui32 id, ui32 severity,
		i32 length, const char* message, const void* userParam) {

		std::stringstream ss;
		ss << "OpenGL :| " << message;

		switch (severity) {
			case GL_DEBUG_SEVERITY_HIGH:
			case GL_DEBUG_SEVERITY_MEDIUM:
				OC_CORE_ERROR(ss.str());
				return;

			case GL_DEBUG_SEVERITY_LOW:
				OC_CORE_WARN(ss.str());
				return;

			case GL_DEBUG_SEVERITY_NOTIFICATION:
				OC_CORE_TRACE(ss.str());
				return;
		}

		OC_CORE_ERROR("Unkown OpenGL severity level!");
	}

	void OpenGLRendererAPI::Init() {
	#ifdef OC_DEBUG
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
	#endif // OC_DEBUG

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(ui32 x, ui32 y, ui32 width, ui32 height) {
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, ui32 indexCount) {
		ui32 count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();

		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}	// Ocean::GL
