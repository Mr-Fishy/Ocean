#include "Window.hpp"

#include "Ocean/Core/Primitives/Log.hpp"
#include "Ocean/Core/Primitives/Numerics.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

static GLFWwindow* s_Window = nullptr;

namespace Ocean {

	static void GLFW_ErrorCallback(i32 error, cstring description) {
		oprint("GLWF Error: %s\n", description);
	}

	void Window::Init(void* config) {
		oprint("Window Service Init\n");

		glfwSetErrorCallback(GLFW_ErrorCallback);

		if (!glfwInit()) {
			oprint("GLFW Init Error!\n");

			return;
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		WindowConfig& windowConfig = *(WindowConfig*)(config);
		s_Window = glfwCreateWindow(windowConfig.Width, windowConfig.Height, windowConfig.Name, glfwGetPrimaryMonitor(), NULL);

		if (!s_Window) {
			oprint("GLFW Window Error!\n");

			return;
		}

		// glfwSetKeyCallback(s_Window, glfw_KeyCallback);

		glfwMakeContextCurrent(s_Window);

		oprint("Window Created Successfully!\n");

		// u32 windowWidth, windowHeight;

		p_PlatformHandle = s_Window;
	}

	void Window::Shutdown() {
		glfwDestroyWindow(s_Window);
		glfwTerminate();

		oprint("Window Service Shutdown\n");
	}

	void Window::SetFullscreen(b8 value) {
		
	}

	void Window::CenterMouse(b8 dragging) const {
		
	}

	void Window::PollEvents() const {
		glfwPollEvents();
	}

}	// Ocean
