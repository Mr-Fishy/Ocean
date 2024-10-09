#include "Application.hpp"

#include "Ocean/Core/Primitives/Log.hpp"

// std
#include <vector>

namespace Ocean {

	Application::Application(const ApplicationConfig& config) {
	}

	Application::~Application() {
	}

	void Application::Close() {
	}

	void Application::Run() {
	#ifdef OC_DEBUG

		// TestApp();

	#endif

		MainLoop();
	}

	void Application::TestApp() {
	}

	b8 Application::MainLoop() { return false; }

	void Application::FixedUpdate(f32 delta) { }

	void Application::VariableUpdate(f32 delta) { }

	void Application::Render(f32 interpolation) { }

	void Application::FrameBegin() { }

	void Application::FrameEnd() { }

}	// Ocean
