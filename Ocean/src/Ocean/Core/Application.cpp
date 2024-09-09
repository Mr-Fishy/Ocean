#include "Application.hpp"

#include "Ocean/Core/Primitives/Log.hpp"

// std
#include <vector>

namespace Ocean {

	Application::Application(const ApplicationConfig& config) {
		oprint("Constructing Ocean Application!\n");
	}

	Application::~Application() {
		oprint("Deconstructing Ocean Application!\n");
	}

	void Application::Close() {
		oprint("Closing Ocean Application!\n");
	}

	void Application::Run() {
	#ifdef OC_DEBUG

		TestApp();

	#endif

		MainLoop();
	}

	void Application::TestApp() {
		oprint("Testing Application Functionality!\n\n");
	}

	b8 Application::MainLoop() { return false; }

	void Application::FixedUpdate(f32 delta) { }

	void Application::VariableUpdate(f32 delta) { }

	void Application::Render(f32 interpolation) { }

	void Application::FrameBegin() { }

	void Application::FrameEnd() { }

}	// Ocean
