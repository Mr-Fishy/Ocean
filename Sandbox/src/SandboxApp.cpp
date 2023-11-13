
#include <Ocean.hpp>

#include "imgui/imgui.h"

// Layers Must Be Declared Before Sandbox Appliction
class ExampleLayer : public Ocean::Layer {
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override {}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(Ocean::Event& event) override {
		// OC_TRACE("{0}", event);
	}
};

// Sandbox Must Be Declared Before Application Initialization
class Sandbox : public Ocean::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

Ocean::Application* Ocean::CreateApplication() {
	return new Sandbox();
}
