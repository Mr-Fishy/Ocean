#pragma once

#ifdef OC_PLATFORM_WINDOWS

extern Ocean::Application* Ocean::CreateApplication();

int main(int argc, char** argv) {
	Ocean::Log::Init();
	OC_CORE_WARN("Initialized Log!");

	auto app = Ocean::CreateApplication();

	try {
		app->Run();
	}
	catch (const std::exception& e) {
		// Report Error
		std::cerr << e.what() << std::endl;

		return EXIT_FAILURE;
	}

	delete app;
	return EXIT_SUCCESS;
}

#endif