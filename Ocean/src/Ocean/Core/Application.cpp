#include "Application.hpp"

#include "Ocean/Core/Primitives/Log.hpp"

// std
#include <vector>

namespace Ocean {

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

}	// Ocean
