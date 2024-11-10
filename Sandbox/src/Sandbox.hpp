#pragma once

// Ocean
#include <Ocean/Ocean.hpp>
#include <Ocean/Core/EntryPoint.hpp>

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

	void OnResize(u16 width, u16 height);

	/* --- */

	// Time

	Ocean::Window* p_Window = nullptr;
	// Input Service

	Ocean::Vulkan::Renderer* p_Renderer = nullptr;

};	// Sandbox



Ocean::Application* Ocean::CreateApplication() {
	Ocean::ApplicationConfig config("Ocean Sandbox", 1200, 800);

	return new Sandbox(config);
}
