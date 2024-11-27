#include "Window.hpp"

#include "Ocean/Core/Primitives/Log.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Ocean {

	static void GLFW_ErrorCallback(i32 error, cstring description) {
		oprint("GLWF Error (%i): %s\n", error, description);
	}

	// TODO: Event calls and input handling

	typedef WindowData* WindowDataPtr;

	static void GLFW_ResizeCallback(GLFWwindow* window, i32 width, i32 height) {
		WindowDataPtr data = static_cast<WindowDataPtr>(glfwGetWindowUserPointer(window));

		data->width = width;
		data->height = height;

		data->resized = true;
	}

	static void GLFW_KeyCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, [[maybe_unused]] i32 mods) {
		WindowDataPtr data = static_cast<WindowDataPtr>(glfwGetWindowUserPointer(window));

		oprint("\t> Window Key-Callback! (%s)\n", glfwGetKeyName(key, scancode));

		if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
			data->window->SetFullscreen(data->fullscreen = !data->fullscreen);

		if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
			data->window->CenterMouse(data->centeredCursor = !data->centeredCursor);
	}

	static void GLFW_CursorCallback(GLFWwindow* window, f64 xpos, f64 ypos) {
		// oprint("\t> Window Cursor-Callback!\n");
	}



	typedef GLFWwindow* WindowPtr;

	void Window::Init(WindowConfig* config) {
		glfwSetErrorCallback(GLFW_ErrorCallback);

		if (!glfwInit()) {
			oprint("GLFW Init Error!\n");
			glfwTerminate();

			return;
		}

		if (!glfwVulkanSupported()) {
			oprint("GLFW Could Not Find Vulkan!\n");
			glfwTerminate();

			return;
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_Data.window = this;
		m_Data.width = config->width;
		m_Data.height = config->height;

		p_PlatformHandle = static_cast<void*>(glfwCreateWindow(m_Data.width, m_Data.height, config->name, NULL, NULL));

		if (!p_PlatformHandle) {
			oprint("GLFW Window Error!\n");

			return;
		}

		glfwSetWindowUserPointer(static_cast<WindowPtr>(p_PlatformHandle), &m_Data);
		glfwSetWindowSizeCallback(static_cast<WindowPtr>(p_PlatformHandle), GLFW_ResizeCallback);
		glfwSetKeyCallback(static_cast<WindowPtr>(p_PlatformHandle), GLFW_KeyCallback);
		glfwSetCursorPosCallback(static_cast<WindowPtr>(p_PlatformHandle), GLFW_CursorCallback);
	}

	void Window::Shutdown() {
		glfwDestroyWindow(static_cast<WindowPtr>(p_PlatformHandle));
		glfwTerminate();
	}

	void Window::SetFullscreen(b8 enabled) {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		if (enabled) {
			m_Data.windowedWidth = m_Data.width;
			m_Data.windowedHeight = m_Data.height;
			oprint("Setting backup size (%i, %i)\n", m_Data.windowedWidth, m_Data.windowedHeight);
			glfwSetWindowMonitor(static_cast<WindowPtr>(p_PlatformHandle), monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		}
		else {
			oprint("Returning to size (%i, %i)\n", m_Data.windowedWidth, m_Data.windowedHeight);
			glfwSetWindowMonitor(static_cast<WindowPtr>(p_PlatformHandle), nullptr, (mode->width / 2) - (m_Data.windowedWidth / 2), (mode->height / 2) - (m_Data.windowedHeight / 2), m_Data.windowedWidth, m_Data.windowedHeight, mode->refreshRate);
		}
	}

	void Window::CenterMouse(b8 enabled) const {
		if (enabled)
			glfwSetInputMode(static_cast<WindowPtr>(p_PlatformHandle), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(static_cast<WindowPtr>(p_PlatformHandle), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void Window::PollEvents() {
		glfwPollEvents();

		if (glfwWindowShouldClose(static_cast<WindowPtr>(p_PlatformHandle)))
			m_RequestedExit = true;
	}

}	// Ocean
