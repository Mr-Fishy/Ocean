#include "ocpch.hpp"

#include "Ocean/Core/Application.hpp"

#include "Ocean/Input/ApplicationEvent.hpp"

#include "Ocean/Renderer/Renderer.hpp"

// libs
#include <GLFW/glfw3.h>

namespace Ocean {

	Application* Application::s_Instance = nullptr;

	Application::Application() : m_Window(nullptr), m_Running(true), m_Minimized(false) {
		if (s_Instance)
			throw std::runtime_error("Application already exists!");

		s_Instance = this;

		m_Window = Window::Create(WindowProps("Ocean App"));
		m_Window->SetEventCallback(OC_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();
	}

	Application::~Application() {
		Renderer::Shutdown();
	}

	void Application::Close() {
		m_Running = false;
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(OC_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(OC_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
			if (e.Handled) {
				break;
			}

			(*it)->OnEvent(e);
		}
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Run() {
		while (m_Running) {
			Update();
		}
	}

	void Application::Update() {
		f32 time = static_cast<f32>(glfwGetTime());
		Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		if (!m_Minimized) {
			for (Layer* layer : m_LayerStack)
				if (layer->IsEnabled())
					layer->OnUpdate(timestep);
		}

		m_Window->Update();
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
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		this->Update();

		return false;
	}

}	// Ocean
