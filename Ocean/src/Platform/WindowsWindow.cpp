#include "ocpch.hpp"

#include "WindowsWindow.hpp"

#include "Ocean/Input/ApplicationEvent.hpp"

namespace Ocean {

	static ui8 s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description) {
		std::cerr << "GLFW : {\'Error\': " << error << ", \'Desc\': " << description << "}" << std::endl;
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) : m_Window(nullptr), m_Context(nullptr) {
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		if (s_GLFWWindowCount == 0) {
			b32 success = glfwInit();

			glfwSetErrorCallback(GLFWErrorCallback);
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title, nullptr, nullptr);
		s_GLFWWindowCount++;

		m_Context = new VulkanContext(m_Window);
		
		glfwSetWindowUserPointer(m_Window, &m_Data);
		// SetVSync(true);

		// Set GLFW Callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			data.EventCallback(event);
		});
	}

	WindowsWindow::~WindowsWindow() {
		delete m_Context;

		glfwDestroyWindow(m_Window);
		s_GLFWWindowCount--;

		if (s_GLFWWindowCount == 0) {
			glfwTerminate();
		}
	}

	void WindowsWindow::Update() {
		glfwPollEvents();

		m_Context->DrawFrame();
	}

	void WindowsWindow::SetVSync(b8 enabled) {
		if (enabled) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}

		m_Data.VSync = enabled;
	}

	b8 WindowsWindow::IsVSync() const {
		return m_Data.VSync;
	}

}	// Ocean
