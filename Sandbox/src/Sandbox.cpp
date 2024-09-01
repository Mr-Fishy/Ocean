#include "Sandbox.hpp"

// Ocean
#include <Ocean/Core/EntryPoint.hpp>
#include "Sandbox.hpp"

Sandbox::Sandbox(const Ocean::ApplicationConfig& config) : Application(config) {
	oprint("Constructing Sandbox Application!\n");

	// ---> Init Primitive Services

	Ocean::MemoryService::Instance()->Init(nullptr);

	Ocean::oTimeServiceInit();

	p_ServiceManager = Ocean::ServiceManager::Instance();
	p_ServiceManager->Init(Ocean::MemoryService::Instance()->SystemAllocator());

	// ---> Init Foundation

	// Window
	Ocean::WindowConfig winConfig{ config.Width, config.Height, config.Name, Ocean::MemoryService::Instance()->SystemAllocator() };
	p_Window = new Ocean::Window();
	p_Window->Init(&winConfig);

	// Input

	// Graphics

	// App Specific 

	// IMGUI
	p_Imgui = p_ServiceManager->Get<Ocean::ImguiService>();
	p_Imgui->Init(nullptr);

	oprint("Successfully Constructed Sandbox Application!\n");
}

Sandbox::~Sandbox() {
	oprint("Deconstructing Sandbox Application!\n");

	p_Imgui->Shutdown();
	// Input
	// Renderer
	p_Window->Shutdown();

	Ocean::oTimeServiceShutdown();

	p_ServiceManager->Shutdown();

	Ocean::MemoryService::Instance()->Shutdown();
}



b8 Sandbox::MainLoop() {
	oprint("Sandbox MainLoop Reached!\n");

	while (!p_Window->RequestedExit()) {
		FrameBegin();




		p_Imgui->NewFrame();

		// Fixed Update

		// Variable Update

		if (!p_Window->Minimized()) {



			p_Imgui->Render();


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
	oprint("Sandbox FixedUpdate!\n");
}

void Sandbox::VariableUpdate(f32 delta) {
	oprint("Sandbox VariableUpdate!\n");
}



void Sandbox::Render(f32 interpolation) {
	oprint("Sandbox Render!\n");
}



void Sandbox::FrameBegin() {
	oprint("Sandbox FrameBegin!\n");
}

void Sandbox::FrameEnd() {
	oprint("Sandbox FrameEnd!\n");
}



void Sandbox::OnResize(u32 width, u32 height) {
	oprint("Sandbox OnResize!\n");
}
