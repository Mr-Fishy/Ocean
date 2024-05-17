#include "ocpch.hpp"

#include "Ocean/Core/Application.hpp"
#include <Input/ApplicationEvent.hpp>

// libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Ocean {

	Application::Application() : m_Window(nullptr), m_Running(true), m_Minimized(false) {
		m_Window = Window::Create();
		m_Window->SetEventCallback(OC_BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application() {
		delete m_Window;
	}

	void Application::Close() {
		m_Running = false;
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(OC_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(OC_BIND_EVENT_FN(Application::OnWindowResize));

		// Propagate event to layers until handled
	}

	void Application::Run() {
		while (m_Running) {
			// TODO: Timestep Stuff

			if (!m_Minimized) {
				// Update Layers
			}

			m_Window->Update();
		}
	}

	b8 Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;

		return true;
	}

	b8 Application::OnWindowResize(WindowResizeEvent& e) {
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;

			return false;
		}

		m_Minimized = false;
		// Resize For Renderer

		return true;
	}

}	// Ocean
