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

};	// Sandbox
