#pragma once

#include "Ocean/Core/Base.hpp"
#include "Ocean/Core/Defines.hpp"

#include "Ocean/Core/Window.hpp"

#include "Ocean/Input/Event.hpp"
#include "Ocean/Input/ApplicationEvent.hpp"

#include "Ocean/Layer/LayerStack.hpp"

#include "Ocean/Core/Timestep.hpp"

int main(int argc, char** argv);

namespace Ocean {

	class Application {
	public:
		Application();
		virtual ~Application();

		void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		const f32 GetLastFrameTime() const { return m_LastFrameTime; }

		static Application& Get() { return *s_Instance; }
		Window& GetWindow() { return *m_Window; }

	private:
		friend int ::main(int argc, char** argv);

		/* --- */

		void Run();

		void Update();

		b8 OnWindowClose(WindowCloseEvent& e);
		b8 OnWindowResize(WindowResizeEvent& e);

		/* --- */

		static Application* s_Instance;

		Scope<Window> m_Window;

		LayerStack m_LayerStack;

		b8 m_Running;
		b8 m_Minimized;

		f32 m_LastFrameTime = 0.0f;
	};

	Application* CreateApplication();

}	// Ocean
