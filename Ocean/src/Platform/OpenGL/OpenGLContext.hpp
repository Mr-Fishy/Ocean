#pragma once

#include "Ocean/Renderer/GraphicsContext.hpp"

struct GLFWwindow;

namespace Ocean {

	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};

}	// Ocean