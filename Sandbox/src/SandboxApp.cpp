
#include <Ocean.hpp>

// Layers Must Be Declared Before Sandbox Appliction
class ExampleLayer : public Ocean::Layer {
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override {}

	void OnEvent(Ocean::Event& event) override {
		OC_TRACE("{0}", event);
	}
};

// Sandbox Must Be Declared Before Application Initialization
class Sandbox : public Ocean::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushOverlay(new Ocean::ImGuiLayer());
	}

	~Sandbox() {

	}
};

Ocean::Application* Ocean::CreateApplication() {
	return new Sandbox();
}
