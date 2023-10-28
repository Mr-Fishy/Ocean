#pragma once

#ifdef OC_PLATFORM_WINDOWS

extern Ocean::Application* Ocean::CreateApplication();

int main(int argc, char** argv) {
	Ocean::Log::Init();
	OC_CORE_WARN("Initialized Log!");
	int a = 5;
	OC_INFO("Hello! Var={0}", a);

	auto app = Ocean::CreateApplication();

	app->Run();

	delete app;
}

#endif