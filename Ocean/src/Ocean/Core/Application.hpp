#pragma once

#include "Ocean/Core/Core.hpp"

#include "Ocean/Core/Window.hpp"
#include "Ocean/Core/LayerStack.hpp"
#include "Ocean/Events/Event.hpp"
#include "Ocean/Events/ApplicationEvent.hpp"

#include "Ocean/Core/Timestep.hpp"

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

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		/* --- */

		static Application* s_Instance;
		Ref<Window> m_Window;

		bool m_Running = true;
		bool m_Minimized = false;

		LayerStack m_LayerStack;

		ImGuiLayer* m_ImGuiLayer;

		float m_LastFrameTime = 0.0f;
	};

	// To Be Defined In CLIENT
	Application* CreateApplication();

}	// Ocean
