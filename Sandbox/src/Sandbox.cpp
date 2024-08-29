#include "Sandbox.hpp"

// Ocean
#include <Ocean/Core/EntryPoint.hpp>
#include "Sandbox.hpp"

Sandbox::Sandbox(const Ocean::ApplicationConfig& config) : Application(config) {
	oprint("Constructing Sandbox Application!\n");

	// ---> Init Primitive Services

	Ocean::MemoryService::Instance()->Init(nullptr);

	// p_ServiceManager = Ocean::ServiceManager::Instance;
	// Init Service Manager

	// ---> Init Foundation

	// Window
	Ocean::WindowConfig winConfig{ config.Width, config.Height, config.Name /* &Ocean::MemoryService::Instance()->SystemAllocator */ };
	p_Window = new Ocean::Window();
	p_Window->Init(&winConfig);

	// Input

	// Graphics

	// App Specific 

	oprint("Successfully Constructed Sandbox Application!\n");
}

Sandbox::~Sandbox() {
	oprint("Deconstructing Sandbox Application!\n");
}



b8 Sandbox::MainLoop() {
	oprint("Sandbox MainLoop Reached!\n");

	return false;
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
