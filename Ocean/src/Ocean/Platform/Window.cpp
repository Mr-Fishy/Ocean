#include "Window.hpp"

#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Log.hpp"

#include "Ocean/Core/Application.hpp"

#include "Ocean/Renderer/RendererAPI.hpp"

// libs
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Ocean {

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

	static void GLFW_CursorCallback(OC_UNUSED GLFWwindow* window, OC_UNUSED f64 xpos, OC_UNUSED f64 ypos) {
		// oprint("\t> Window Cursor-Callback!\n");
	}



	typedef GLFWwindow* WindowPtr;

	Window::Window(u32 width, u32 height, cstring name) : m_Context(), p_PlatformHandle(nullptr), m_Name(name), m_Data(width, height) {

	}

	void Window::Init() {
		#ifdef OC_DEBUG

		if (Splash::RendererAPI::GetAPI() == Splash::RendererAPI::OpenGL)
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

		#endif

		if (Splash::RendererAPI::GetAPI() == Splash::RendererAPI::OpenGL) {
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		}
		else if (Splash::RendererAPI::GetAPI() == Splash::RendererAPI::Vulkan)
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		this->m_Data.window = this;

		this->p_PlatformHandle = static_cast<void*>(glfwCreateWindow(this->m_Data.width, this->m_Data.height, this->m_Name, NULL, NULL));
		if (!this->p_PlatformHandle) {
			oprint("GLFW Window Error!\n");

			return;
		}

		this->m_Context = Splash::GraphicsContext::Create(this->p_PlatformHandle);
		this->m_Context->Init();

		glfwSetWindowUserPointer(static_cast<WindowPtr>(this->p_PlatformHandle), &m_Data);
		glfwSetWindowSizeCallback(static_cast<WindowPtr>(this->p_PlatformHandle), GLFW_ResizeCallback);
		glfwSetKeyCallback(static_cast<WindowPtr>(this->p_PlatformHandle), GLFW_KeyCallback);
		glfwSetCursorPosCallback(static_cast<WindowPtr>(this->p_PlatformHandle), GLFW_CursorCallback);
	}

	void Window::Shutdown() {
		glfwDestroyWindow(static_cast<WindowPtr>(this->p_PlatformHandle));
	}

	void Window::SetFullscreen(b8 enabled) {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		if (enabled) {
			this->m_Data.windowedWidth = this->m_Data.width;
			this->m_Data.windowedHeight = this->m_Data.height;
			oprint("Setting backup size (%i, %i)\n", this->m_Data.windowedWidth, this->m_Data.windowedHeight);
			glfwSetWindowMonitor(static_cast<WindowPtr>(this->p_PlatformHandle), monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		}
		else {
			oprint("Returning to size (%i, %i)\n", this->m_Data.windowedWidth, this->m_Data.windowedHeight);
			glfwSetWindowMonitor(static_cast<WindowPtr>(this->p_PlatformHandle), nullptr, (mode->width / 2) - (this->m_Data.windowedWidth / 2), (mode->height / 2) - (this->m_Data.windowedHeight / 2), m_Data.windowedWidth, this->m_Data.windowedHeight, mode->refreshRate);
		}
	}

	void Window::CenterMouse(b8 enabled) const {
		if (enabled)
			glfwSetInputMode(static_cast<WindowPtr>(this->p_PlatformHandle), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(static_cast<WindowPtr>(this->p_PlatformHandle), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void Window::OnUpdate() {
		glfwPollEvents();

		if (Resized()) {
			Application::Get()->OnResize(this->m_Data.width, this->m_Data.height);

			ResizeHandled();
		}

		if (glfwWindowShouldClose(static_cast<WindowPtr>(this->p_PlatformHandle)))
			this->m_RequestedExit = true;

		this->m_Context->SwapBuffers();
	}

	Scope<Window> Window::Create(u32 width, u32 height, cstring name) {
		return MakeScope<Window>(width, height, name);
	}

}	// Ocean
