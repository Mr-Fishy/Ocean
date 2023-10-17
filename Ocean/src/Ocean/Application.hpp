#pragma once

#include "Core.hpp"

namespace Ocean {

	class OCEAN_API Application {
	public:
		Application();
		virtual~Application();

		void Run();
	};

	// To Be Defined In CLIENT
	Application* CreateApplication();

}

