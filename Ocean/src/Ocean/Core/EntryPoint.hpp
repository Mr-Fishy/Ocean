#pragma once

// Ocean
#include "Ocean/Core/Application.hpp"
#include "Ocean/Core/Primitives/Memory.hpp"

// std
#include <iostream>

extern Ocean::Application* Ocean::CreateApplication();

int main(int argc, char** argv) {
	Ocean::oTimeServiceInit();
	Ocean::MemoryService::Instance().Init(nullptr);

	Ocean::Application* app = Ocean::CreateApplication();

	try {
		app->Run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;

		delete app;

		Ocean::MemoryService::Shutdown();
		Ocean::oTimeServiceInit();
		return EXIT_FAILURE;
	}

	delete app;

	Ocean::MemoryService::Shutdown();
	Ocean::oTimeServiceInit();
	return EXIT_SUCCESS;
}
