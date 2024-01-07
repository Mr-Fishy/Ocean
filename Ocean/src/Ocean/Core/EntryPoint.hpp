#pragma once

#include "Ocean/Core/Base.hpp"

#include "Ocean/Core/Memory/MemoryManager.hpp"

#ifdef OC_PLATFORM_WINDOWS

extern Ocean::Application* Ocean::CreateApplication();

int main(int argc, char** argv) {
	Ocean::Log::Init();   // Initializes spdlog log system.

	OC_PROFILE_BEGIN_SESSION("Startup", "OceanProfile-Startup.json");
	auto app = Ocean::CreateApplication();
	OC_PROFILE_END_SESSION();

	try {
		OC_PROFILE_BEGIN_SESSION("Runtime", "OceanProfile-Runtime.json");

		// Allocate 3, is aligned to 8
		auto p1 = Ocean::MemoryManager::alloc(3);
		auto p1b = Ocean::MemoryManager::getHeader(p1);
		OC_CORE_INFO("Allocated: {0}, Success: {1}", 3, p1b->size == sizeof(Ocean::MemoryManager::word_t));

		// Allocate 8, is aligned to 8
		auto p2 = Ocean::MemoryManager::alloc(8);
		auto p2b = Ocean::MemoryManager::getHeader(p2);
		OC_CORE_INFO("Allocated: {0}, Success: {1}", 8, p2b->size == 8);

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
