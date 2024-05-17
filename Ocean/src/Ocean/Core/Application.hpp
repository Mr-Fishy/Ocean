#pragma once

#include "Ocean/Core/Defines.hpp"

#include "Ocean/Core/Window.hpp"

#include "Ocean/Input/Event.hpp"
#include "Ocean/Input/ApplicationEvent.hpp"

int main(int argc, char** argv);

namespace Ocean {

	const ui32 WIDTH = 1600;
	const ui32 HEIGHT = 900;

	class Application {
	public:
		Application();
		virtual ~Application();

		void Close();

		void OnEvent(Event& e);

	private:
		friend int ::main(int argc, char** argv);

		/* --- */

		void Run();

		b8 OnWindowClose(WindowCloseEvent& e);
		b8 OnWindowResize(WindowResizeEvent& e);

		/* --- */

		Window* m_Window;

		b8 m_Running;
		b8 m_Minimized;
	};

	Application* CreateApplication();

}	// Ocean
