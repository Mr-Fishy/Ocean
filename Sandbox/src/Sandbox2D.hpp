#pragma once

#include <Ocean/Ocean.hpp>

class Sandbox2D : public Ocean::Layer {
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Timestep ts) override;

	// virtual void OnEvent(Ocean::Event& e) override;

private:
    Ocean::OrthoCamera m_Camera;

};  // Sandbox2D
