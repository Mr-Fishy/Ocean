#pragma once

#include "Ocean/Renderer/GraphicsContext.hpp"

struct GLFWwindow;

namespace Ocean::GL {

	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override final;
		virtual void SwapBuffers() override final;

	private:
		GLFWwindow* m_WindowHandle;
	};

}	// Ocean::GL
