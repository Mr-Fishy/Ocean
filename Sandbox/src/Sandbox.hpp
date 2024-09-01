#pragma once

// Ocean
#include <Ocean/Ocean.hpp>

class Sandbox : public Ocean::Application {
public:
	Sandbox(const Ocean::ApplicationConfig& config);
	~Sandbox();

private:
	virtual b8 MainLoop() override final;

	virtual void FixedUpdate(f32 delta) override final;
	virtual void VariableUpdate(f32 delta);

	virtual void Render(f32 interpolation) override final;

	virtual void FrameBegin() override final;
	virtual void FrameEnd() override final;

	void OnResize(u32 width, u32 height);

	/* --- */

	// Time

	Ocean::Window* p_Window = nullptr;
	// Input Service

	// Renderer Service

	Ocean::ImguiService* p_Imgui = nullptr;

};	// Sandbox



Ocean::Application* Ocean::CreateApplication() {
	Ocean::ApplicationConfig config;
	config.w(1080);
	config.h(720);
	config.name("Ocean Sandbox");

	return new Sandbox(config);
}
