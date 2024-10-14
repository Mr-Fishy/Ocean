#include "Sandbox.hpp"

// Ocean
#include <Ocean/Core/EntryPoint.hpp>

static Ocean::Window s_Window;

Sandbox::Sandbox(const Ocean::ApplicationConfig& config) : Application(config) {
	oprint(CONSOLE_TEXT_CYAN("Constructing Sandbox Application!\n"));

	// ---> Init Primitive Services

	Ocean::MemoryService::Instance()->Init(nullptr);

	Ocean::oTimeServiceInit();

	p_ServiceManager = Ocean::ServiceManager::Instance();
	p_ServiceManager->Init(Ocean::MemoryService::Instance()->SystemAllocator());

	// ---> Init Foundation

	// Window
	Ocean::WindowConfig winConfig{
		config.Width,
		config.Height,
		config.Name,
		Ocean::MemoryService::Instance()->SystemAllocator()
	};
	p_Window = &s_Window;
	p_Window->Init(&winConfig);

	// Input

	// Graphics
	Ocean::Vulkan::RendererConfig renConfig{
		Ocean::MemoryService::Instance()->SystemAllocator(),
		p_Window,
		config.Name,
		(u16)p_Window->Width(),
		(u16)p_Window->Height(),
		2,
		1, 0, 0
	};
	p_Renderer = p_ServiceManager->Get<Ocean::Vulkan::Renderer>();
	p_Renderer->Init(&renConfig);
}

Sandbox::~Sandbox() {
	oprint(CONSOLE_TEXT_CYAN("Deconstructing Sandbox Application!\n"));

	// Graphics
	p_Renderer->Shutdown();

	// Input

	// Window
	p_Window->Shutdown();

	Ocean::oTimeServiceShutdown();

	p_ServiceManager->Shutdown();

	Ocean::MemoryService::Instance()->Shutdown();
}



b8 Sandbox::MainLoop() {
	while (!p_Window->RequestedExit()) {
		if (!p_Window->Minimized())
			p_Renderer->BeginFrame();

		FrameBegin();

		p_Window->PollEvents();

		if (p_Window->Resized()) {
			p_Renderer->ResizeSwapchain(p_Window->Width(), p_Window->Height());

			OnResize(p_Window->Width(), p_Window->Height());

			p_Window->ResizeHandled();
		}

		// Fixed Update

		// Variable Update

		if (!p_Window->Minimized()) {
			// Command Buffer

			// Interpolation

			Render(f32());

			p_Renderer->EndFrame();
		}

		// Prepare for the next frame if needed.
		FrameEnd();
	}

	p_Renderer->CleanUp();

	return true;
}



void Sandbox::FixedUpdate(f32 delta) {
}

void Sandbox::VariableUpdate(f32 delta) {
}



void Sandbox::Render(f32 interpolation) {
	p_Renderer->RenderFrame();
}



void Sandbox::FrameBegin() {
}

void Sandbox::FrameEnd() {
}



void Sandbox::OnResize(u16 width, u16 height) {
	p_Renderer->ResizeSwapchain(width, height);
}
