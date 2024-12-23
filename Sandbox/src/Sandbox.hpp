#pragma once

// Ocean
#include <Ocean/Ocean.hpp>
#include <Ocean/Core/EntryPoint.hpp>

class Sandbox : public Ocean::Application {
public:
	Sandbox(const Ocean::ApplicationConfig& config);
	~Sandbox();

private:
	Sandbox(const Sandbox&) = delete;
	Sandbox& operator = (const Sandbox&) = delete;

	virtual b8 MainLoop() override final;

	virtual void FixedUpdate(Timestep delta) override final;
	virtual void VariableUpdate(Timestep delta) override final;

	virtual void Render(f32 interpolation) override final;

	virtual void FrameBegin() override final;
	virtual void FrameEnd() override final;

	void OnResize(u16 width, u16 height);

	/* --- */

	// Time

	static inline Ocean::Window* p_Window = nullptr;
	// Input Service

};	// Sandbox
