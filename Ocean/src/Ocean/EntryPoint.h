#pragma once

#ifdef OC_PLATFORM_WINDOWS

extern Ocean::Application* Ocean::CreateApplication();

int main(int argc, char** argv) {
	auto app = Ocean::CreateApplication();

	app->Run();

	delete app;
}

#endif