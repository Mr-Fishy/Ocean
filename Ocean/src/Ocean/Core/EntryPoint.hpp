#pragma once

#include "Ocean/Core/Base.hpp"

#include "Ocean/Memory/FreeListAllocator.hpp"

// std
#include <iostream>

#ifdef OC_PLATFORM_WINDOWS

extern Ocean::Application* Ocean::CreateApplication();

int main(int argc, char** argv) {
	using namespace Ocean;

	Log::Init();   // Initializes spdlog log system.

	OC_PROFILE_BEGIN_SESSION("Startup", "OceanProfile-Startup.json");
	Application* app = CreateApplication();
	OC_PROFILE_END_SESSION();

	try {
		OC_PROFILE_BEGIN_SESSION("Runtime", "OceanProfile-Runtime.json");

		app->Run();

		OC_PROFILE_END_SESSION();
	}
	catch (const std::exception& e) {
		// Report Error
		std::cerr << e.what() << std::endl;

		delete app;
		return EXIT_FAILURE;
	}

	OC_PROFILE_BEGIN_SESSION("Shutdown", "OceanProfile-Shutdown.json");
	delete app;
	OC_PROFILE_END_SESSION();

	return EXIT_SUCCESS;
}

#endif
