#pragma once

// Ocean
#include "Ocean/Primitives/Exceptions.hpp"
#include "Ocean/Primitives/Memory.hpp"

#include "Ocean/Core/Application.hpp"

// std
#include <exception>
#include <iostream>

OC_EXTERN Ocean::Application* Ocean::CreateApplication(int argc, char** argv);

int main(int argc, char** argv) {
	oTimeServiceInit();
	MemoryService::Instance().Init(nullptr);

	Ocean::Application* app = Ocean::CreateApplication(argc, argv);

	try {
		app->Run();
	}
	catch (const Ocean::Exception& e) {
		std::cerr << "Ocean Exception: " << e.what() << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	delete app;

	MemoryService::Shutdown();
	oTimeServiceInit();
	return EXIT_SUCCESS;
}
