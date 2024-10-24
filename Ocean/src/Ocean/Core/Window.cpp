#include "Window.hpp"

#include "Ocean/Core/Primitives/Log.hpp"
#include "Ocean/Core/Primitives/Numerics.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Ocean {

	static void GLFW_ErrorCallback(i32 error, cstring description) {
		oprint("GLWF Error: %s\n", description);
	}

	// TODO: Event calls and input handling

	typedef WindowData* WindowDataPtr;

	static void GLFW_RefreshCallback(GLFWwindow* window) {
		WindowDataPtr data = (WindowDataPtr)glfwGetWindowUserPointer(window);

		oprint("\t> Window Refreshed!\n");
		data->Refreshed = true;
	}

	static void GLFW_ResizeCallback(GLFWwindow* window, i32 width, i32 height) {
		WindowDataPtr data = (WindowDataPtr)glfwGetWindowUserPointer(window);

		data->Width = width;
		data->Height = height;

		oprint("\t> Window Resized! (%i, %i)\n", width, height);
		data->Resized = true;
	}

	static void GLFW_KeyCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods) {
		WindowDataPtr data = (WindowDataPtr)glfwGetWindowUserPointer(window);

		oprint("\t> Window Key-Callback! (%s)\n", glfwGetKeyName(key, scancode));

		if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
			data->Window->SetFullscreen(data->Fullscreen = !data->Fullscreen);

		if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
			data->Window->CenterMouse(data->CenteredCursor = !data->CenteredCursor);
	}

	static void GLFW_CursorCallback(GLFWwindow* window, f64 xpos, f64 ypos) {
		// oprint("\t> Window Cursor-Callback!\n");
	}



	typedef GLFWwindow* WindowPtr;

	void Window::Init(void* config) {
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

		WindowConfig& windowConfig = *(WindowConfig*)(config);

		m_Data.Window = this;
		m_Data.Width = windowConfig.Width;
		m_Data.Height = windowConfig.Height;

		p_PlatformHandle = (void*)glfwCreateWindow(m_Data.Width, m_Data.Height, windowConfig.Name, NULL, NULL);

		if (!p_PlatformHandle) {
			oprint("GLFW Window Error!\n");

			return;
		}

		glfwSetWindowUserPointer((WindowPtr)p_PlatformHandle, &m_Data);
		// glfwSetWindowRefreshCallback((WindowPtr)p_PlatformHandle, GLFW_RefreshCallback);
		glfwSetWindowSizeCallback((WindowPtr)p_PlatformHandle, GLFW_ResizeCallback);
		glfwSetKeyCallback((WindowPtr)p_PlatformHandle, GLFW_KeyCallback);
		glfwSetCursorPosCallback((WindowPtr)p_PlatformHandle, GLFW_CursorCallback);
	}

	void Window::Shutdown() {
		glfwDestroyWindow((WindowPtr)p_PlatformHandle);
		glfwTerminate();
	}

	void Window::SetFullscreen(b8 enabled) {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		if (enabled) {
			m_Data.WindowedWidth = m_Data.Width;
			m_Data.WindowedHeight = m_Data.Height;
			oprint("Setting backup size (%i, %i)\n", m_Data.WindowedWidth, m_Data.WindowedHeight);
			glfwSetWindowMonitor((WindowPtr)p_PlatformHandle, monitor, NULL, NULL, mode->width, mode->height, mode->refreshRate);
		}
		else {
			oprint("Returning to size (%i, %i)\n", m_Data.WindowedWidth, m_Data.WindowedHeight);
			glfwSetWindowMonitor((WindowPtr)p_PlatformHandle, NULL, (mode->width / 2) - (m_Data.WindowedWidth / 2), (mode->height / 2) - (m_Data.WindowedHeight / 2), m_Data.WindowedWidth, m_Data.WindowedHeight, NULL);
		}
	}

	void Window::CenterMouse(b8 enabled) const {
		if (enabled)
			glfwSetInputMode((WindowPtr)p_PlatformHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode((WindowPtr)p_PlatformHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void Window::PollEvents() {
		glfwPollEvents();

		if (glfwWindowShouldClose((WindowPtr)p_PlatformHandle))
			m_RequestedExit = true;
	}

}	// Ocean
