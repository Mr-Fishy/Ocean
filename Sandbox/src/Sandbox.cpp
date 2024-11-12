#include "Sandbox.hpp"
#include "Ocean/Core/Primitives/Memory.hpp"

Ocean::Application* Ocean::CreateApplication() {
	Ocean::ApplicationConfig config("Ocean Sandbox", 1200, 800);

	return new Sandbox(config);
}



static Ocean::Window s_Window;



Sandbox::Sandbox(const Ocean::ApplicationConfig& config) : Application(config) {
	oprint(CONSOLE_TEXT_CYAN("Constructing Sandbox Application!\n"));

	// ---> Init Primitive Services

	Ocean::MemoryService::Instance().Init(nullptr);

	Ocean::oTimeServiceInit();

	p_ServiceManager = &Ocean::ServiceManager::Instance();
	p_ServiceManager->Init();

	// ---> Init Foundation

	// Window
	Ocean::WindowConfig winConfig {
		config.width,
		config.height,
		config.name,
	};
	p_Window = static_cast<Ocean ::Window *>(Ocean ::MemoryService ::Instance().SystemAllocator()->Allocate(sizeof(Ocean ::Window) * 1, alignof(Ocean ::Window), "c:\\dev\\Ocean\\Sandbox\\src\\Sandbox.cpp", 36));

	p_Window->Init(&winConfig);

	// Input

	// Graphics
	Ocean::Vulkan::RendererConfig renConfig{
		Ocean::MemoryService::Instance().SystemAllocator(),
		p_Window,
		config.name,
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
	ofree(p_Window, Ocean::MemoryService::Instance().SystemAllocator());

	Ocean::ServiceManager::Shutdown();

	Ocean::oTimeServiceShutdown();

	Ocean::MemoryService::Shutdown();
}



b8 Sandbox::MainLoop() {
	Timestep currentTime(Ocean::oTimeRealiSec(Ocean::oTimeNow()));
	Timestep accumulator(0.0f);

	Timestep dt(0.01f);

	while (!p_Window->RequestedExit()) {
		Timestep t(Ocean::oTimeRealiSec(Ocean::oTimeNow()));
		Timestep frameTime(t - currentTime);
		currentTime = t;

		accumulator += frameTime;

		if (!p_Window->Minimized())
			p_Renderer->BeginFrame();

		FrameBegin();

		p_Window->PollEvents();

		if (p_Window->Resized()) {
			OnResize(p_Window->Width(), p_Window->Height());

			p_Window->ResizeHandled();
		}

		// Fixed Update
		while (accumulator >= dt) {
			// TODO: Interpolation (For Physics Engine)
			FixedUpdate(dt);

			accumulator -= dt;
		}

		// Variable Update
		VariableUpdate(frameTime);

		if (!p_Window->Minimized()) {
			// TODO: Interpolation
			Render(f32());

			p_Renderer->EndFrame();
		}

		// Prepare for the next frame if needed.
		FrameEnd();
	}

	p_Renderer->CleanUp();

	Close();

	return true;
}



void Sandbox::FixedUpdate(Timestep delta) {
}

void Sandbox::VariableUpdate(Timestep delta) {
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
