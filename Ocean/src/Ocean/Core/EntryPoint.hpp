#pragma once

#include "Ocean/Core/PlatformBase.hpp"
#include "Ocean/Core/Application.hpp"

// std
#include <iostream>

#ifdef OC_PLATFORM_WINDOWS

	extern Ocean::Application* Ocean::CreateApplication();

	int main(int argc, char** argv) {
		// TODO: Logger
		
		Ocean::Application* app = Ocean::CreateApplication();

		try {
			app->Run();
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;

			return EXIT_FAILURE;
		}

		return EXIT_SUCCESS;
	}

#endif // OC_PLATFORM_WINDOWS
