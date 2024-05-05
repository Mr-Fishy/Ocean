
#include <Ocean.hpp>
/* --- Entry Point --- */
#include <Ocean/Core/EntryPoint.hpp>

/* --- Renderable Layers --- */
#include "Sandbox2D.hpp"

// Layers Must Be Declared Before Sandbox Appliction

// Sandbox Must Be Declared Before Application Initialization
class Sandbox : public Ocean::Application {
public:
	Sandbox() {
		// PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox() {

	}
};

Ocean::Application* Ocean::CreateApplication() {
	return new Sandbox();
}
