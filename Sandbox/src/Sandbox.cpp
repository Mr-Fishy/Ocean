#include "Sandbox.hpp"

#include "Ocean/Core/Application.hpp"

#include "Ocean/Core/Primitives/Macros.hpp"

#include "Ocean/Core/Window.hpp"
#include "Renderer/Renderer.hpp"

Ocean::Application* Ocean::CreateApplication() {
	Ocean::ApplicationConfig config("Ocean Sandbox", 1200, 800);

	return new Sandbox(config);
}



Sandbox::Sandbox(const Ocean::ApplicationConfig& config) : Ocean::Application(config), m_Window() {
	oprint(CONSOLE_TEXT_CYAN("Constructing Sandbox Application!\n"));

	this->m_Window = Ocean::Window::Create(config.width, config.height, config.name);
	this->m_Window->Init();

	Ocean::Shrimp::Renderer::Init();
}

Sandbox::~Sandbox() {
	oprint(CONSOLE_TEXT_CYAN("Deconstructing Sandbox Application!\n"));

	Ocean::Shrimp::Renderer::Shutdown();
	
}



void Sandbox::FixedUpdate(OC_UNUSED Timestep delta) {

}

void Sandbox::VariableUpdate(OC_UNUSED Timestep delta) {
}



void Sandbox::FrameBegin() {
}

void Sandbox::FrameEnd() {
}
