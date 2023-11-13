#pragma once

#include "Core.hpp"

#include "Window.hpp"
#include "Ocean/LayerStack.hpp"
#include "Ocean/Events/Event.hpp"
#include "Ocean/Events/ApplicationEvent.hpp"

#include "Ocean/ImGui/ImGuiLayer.hpp"

namespace Ocean {

	class OCEAN_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		static inline Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		/* --- */

		std::unique_ptr<Window> m_Window;

		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;

		LayerStack m_LayerStack;

		static Application* s_Instance;
	};

	// To Be Defined In CLIENT
	Application* CreateApplication();

}

