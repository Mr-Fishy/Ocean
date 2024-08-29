#pragma once

// Ocean
#include "Ocean/Core/Base.hpp"
#include "Ocean/Core/Application.hpp"

// std
#include <iostream>

#ifdef OC_PLATFORM_WINDOWS

	extern Ocean::Application* Ocean::CreateApplication();

	int main(int argc, char** argv) {
		Ocean::Application* app = Ocean::CreateApplication();

		try {
			app->Run();
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;

			delete app;
			return EXIT_FAILURE;
		}

		delete app;
		return EXIT_SUCCESS;
	}

#endif // OC_PLATFORM_WINDOWS
