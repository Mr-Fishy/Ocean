#include "ocpch.hpp"

#include "Platform/OpenGL/OpenGLContext.hpp"

// libs
#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace Ocean::GL {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {
		if (!m_WindowHandle)
			throw std::runtime_error("Window handle is null!");
	}

	void OpenGLContext::Init() {
		glfwMakeContextCurrent(m_WindowHandle);

		i32 status = gladLoadGL((GLADloadfunc)glfwGetProcAddress);
		if (!status)
			throw std::runtime_error("Failed to initialize glad::gl!");
	}

	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(m_WindowHandle);
	}

}	// Ocean::GL
