#pragma once

// Ocean
#include "Ocean/Core/Types/UniquePtr.hpp"
#include "Ocean/Core/Window.hpp"
#include <Ocean/Ocean.hpp>
#include <Ocean/Core/EntryPoint.hpp>

class Sandbox : public Ocean::Application {
public:
	Sandbox(const Ocean::ApplicationConfig& config);
	~Sandbox();

private:
	Sandbox(const Sandbox&) = delete;
	Sandbox& operator = (const Sandbox&) = delete;

	virtual void FixedUpdate(Timestep delta) override final;
	virtual void VariableUpdate(Timestep delta) override final;

	virtual void FrameBegin() override final;
	virtual void FrameEnd() override final;

	/* --- */

	UniquePtr<Ocean::Window> m_Window;

};	// Sandbox
