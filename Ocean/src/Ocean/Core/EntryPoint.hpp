#pragma once

#include "Ocean/Core/Base.hpp"

#ifdef OC_PLATFORM_WINDOWS

extern Ocean::Application* Ocean::CreateApplication();

int main(int argc, char** argv) {
	Ocean::Log::Init();

	OC_PROFILE_BEGIN_SESSION("Startup", "OceanProfile-Startup.json");
	auto app = Ocean::CreateApplication();
	OC_PROFILE_END_SESSION();
	try {
		OC_PROFILE_BEGIN_SESSION("Runtime", "OceanProfile-Runtime.json");
		app->Run();
		OC_PROFILE_END_SESSION();
	}
	catch (const std::exception& e) {
		// Report Error
		std::cerr << e.what() << std::endl;

		return EXIT_FAILURE;
	}

	OC_PROFILE_BEGIN_SESSION("Shutdown", "OceanProfile-Startup.json");
	delete app;
	OC_PROFILE_END_SESSION();
	return EXIT_SUCCESS;
}

#endif
