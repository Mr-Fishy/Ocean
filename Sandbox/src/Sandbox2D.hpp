#pragma once

#include "Ocean.hpp"

class Sandbox2D : public Ocean::Layer {
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnImGuiRender(Ocean::Timestep ts) override;
	void OnUpdate(Ocean::Timestep ts) override;
	void OnEvent(Ocean::Event& e) override;

private:
	Ocean::OrthographicCameraController m_CameraController;

	// TEMP
	Ocean::Ref<Ocean::VertexArray> m_SquareVA;
	Ocean::Ref<Ocean::Shader> m_FlatColorShader;

	Ocean::Ref<Ocean::Texture2D> m_CheckerboardTexture;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};
