
#include "ocpch.hpp"
#include "OpenGLContext.hpp"

// libs
#include <GLFW/glfw3.h>
#include <glad/gl.h>

namespace Ocean {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {
		OC_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init() {
		OC_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);

		int status = gladLoadGL((GLADloadfunc)glfwGetProcAddress);
		OC_CORE_ASSERT(status, "Failed to initialize Glad!");

		OC_CORE_INFO("OpenGL Info:");
		OC_CORE_INFO("- Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		OC_CORE_INFO("- Version: {0}", (const char*)glGetString(GL_VERSION));

		#ifdef OC_ENABLE_ASSERTS
			int versionMajor, versionMinor;
		
			glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
			glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

			OC_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Ocean Engine requires at least OpenGL version 4.5!");
		#endif
	}

	void OpenGLContext::SwapBuffers() {
		OC_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}

}	// Ocean