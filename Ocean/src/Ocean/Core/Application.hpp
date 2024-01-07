#pragma once

#include "Ocean/Core/Base.hpp"

#include "Ocean/Core/Window.hpp"
#include "Ocean/Core/Layers/LayerStack.hpp"
#include "Ocean/Events/Event.hpp"
#include "Ocean/Events/ApplicationEvent.hpp"

#include "Ocean/Core/Timestep.hpp"

#include "Ocean/ImGui/ImGuiLayer.hpp"

int main(int argc, char** argv);

namespace Ocean {

	// The application represents the actual process that is seen on screen.
	//
	class Application {
	public:
		// Creates an application with a given name.
		// @param name: A string that is the application name. Defaults to "Ocean App".
		//
		Application(const std::string& name = "Ocean App");
		virtual ~Application();

		void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
		static Application& Get() { return *s_Instance; }
		Window& GetWindow() { return *m_Window; }

	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		/* --- */

		friend int ::main(int argc, char** argv);

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
	//
	Application* CreateApplication();

}	// Ocean
