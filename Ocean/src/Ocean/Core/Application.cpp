
#include "ocpch.hpp"
#include "Application.hpp"

#include "Ocean/Renderer/Renderer.hpp"

#include "Ocean/Core/Input.hpp"

//libs
#include <GLFW/glfw3.h>

namespace Ocean {

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name) {
		OC_PROFILE_FUNCTION();

		OC_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(OC_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application() {
		OC_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	float Application::GetDPI(int monitorIndex) {
		int monitorCount;
		GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);

		float x, y;

		if (monitorCount > 1) {
			OC_CORE_INFO("Multiple Monitors Connected, Using Primary Monitor.");
			glfwGetMonitorContentScale(monitors[0], &x, &y);
		}
		else if (monitorCount == 1) {
			glfwGetMonitorContentScale(monitors[0], &x, &y);
		}
		else {
			OC_CORE_ASSERT("No Monitors Are Connected According To GLFW!");
		}

		return (x + y) / 2;
	}

	void Application::Run() {
		OC_PROFILE_FUNCTION();

		while (m_Running) {
			OC_PROFILE_SCOPE("RuntimeLoop");

			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized) {
				{
					OC_PROFILE_SCOPE("Layerstack OnUpdate");

					for (Layer* layer : m_LayerStack) {
						layer->OnUpdate(timestep);
					}
				}
			}

			m_ImGuiLayer->Begin();

			{
				OC_PROFILE_SCOPE("Layerstack OnImGuiRender");

				for (Layer* layer : m_LayerStack) {
					layer->OnImGuiRender();
				}
			}

			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::Close() {
		m_Running = false;
	}

	void Application::OnEvent(Event& e) {
		OC_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(OC_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(OC_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled) {
				break;
			}
		
			(*it)->OnEvent(e);
		}
	}

	void Application::PushLayer(Layer* layer) {
		OC_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {
		OC_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e) {
		OC_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;

			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

}	// Ocean
