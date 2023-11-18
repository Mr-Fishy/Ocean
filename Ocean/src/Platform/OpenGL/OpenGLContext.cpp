
#include "ocpch.hpp"
#include "OpenGLContext.hpp"

#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <gl/GL.h>

namespace Ocean {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {
		OC_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init() {
		glfwMakeContextCurrent(m_WindowHandle);

		int status = gladLoadGL((GLADloadfunc)glfwGetProcAddress);
		OC_CORE_ASSERT(status, "Failed to initialize Glad!");

		OC_CORE_INFO("OpenGL Info:");
		OC_CORE_INFO("- Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		OC_CORE_INFO("- Version: {0}", (const char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(m_WindowHandle);
	}

}	// Ocean