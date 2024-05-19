#pragma once

#include "Ocean/Core/PlatformBase.hpp"
#include "Ocean/Core/Application.hpp"
#include "Ocean/Logger/Log.hpp"

// std
#include <iostream>

#ifdef OC_PLATFORM_WINDOWS

	extern Ocean::Application* Ocean::CreateApplication();

	int main(int argc, char** argv) {
		Ocean::Log::Init();
		
		Ocean::Application* app = Ocean::CreateApplication();

		try {
			app->Run();
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;

			delete app;
			Ocean::Log::Shutdown();

			return EXIT_FAILURE;
		}

		delete app;
		Ocean::Log::Shutdown();

		return EXIT_SUCCESS;
	}

#endif // OC_PLATFORM_WINDOWS
