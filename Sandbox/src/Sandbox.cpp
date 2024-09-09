#include "Sandbox.hpp"

// Ocean
#include <Ocean/Core/EntryPoint.hpp>

static Ocean::Window s_Window;

Sandbox::Sandbox(const Ocean::ApplicationConfig& config) : Application(config) {
	oprint("Constructing Sandbox Application!\n");

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
		1, 0, 0
	};
	p_Renderer = p_ServiceManager->Get<Ocean::Vulkan::Renderer>();
	p_Renderer->Init(&renConfig);

	// IMGUI
	// p_Imgui = p_ServiceManager->Get<Ocean::ImguiService>();
	// p_Imgui->Init(nullptr);

	oprint("Successfully Constructed Sandbox Application!\n");
}

Sandbox::~Sandbox() {
	oprint("Deconstructing Sandbox Application!\n");

	// p_Imgui->Shutdown();

	// Graphics
	p_Renderer->Shutdown();

	// Input

	p_Window->Shutdown();

	Ocean::oTimeServiceShutdown();

	p_ServiceManager->Shutdown();

	Ocean::MemoryService::Instance()->Shutdown();
}



b8 Sandbox::MainLoop() {
	oprint("Sandbox MainLoop Reached!\n");

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

		// p_Imgui->NewFrame();

		// Fixed Update

		// Variable Update

		if (!p_Window->Minimized()) {
			// Command Buffer

			// Interpolation

			Render(f32());

			// ImGui->Render()

			p_Renderer->EndFrame();
		}
		else {
			// ImGui::Render();
		}

		// Prepare for the next frame if needed.
		FrameEnd();
	}

	return true;
}



void Sandbox::FixedUpdate(f32 delta) {
	// oprint("Sandbox FixedUpdate!\n");
}

void Sandbox::VariableUpdate(f32 delta) {
	// oprint("Sandbox VariableUpdate!\n");
}



void Sandbox::Render(f32 interpolation) {
	// oprint("Sandbox Render!\n");
}



void Sandbox::FrameBegin() {
	// oprint("Sandbox FrameBegin!\n");
}

void Sandbox::FrameEnd() {
	// oprint("Sandbox FrameEnd!\n");
}



void Sandbox::OnResize(u32 width, u32 height) {
	oprint("Sandbox OnResize! (%i, %i)\n", width, height);
}
