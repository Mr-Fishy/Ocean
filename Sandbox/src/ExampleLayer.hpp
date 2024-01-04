#pragma once

#include "Ocean.hpp"

class ExampleLayer : public Ocean::Layer {
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnImGuiRender() override;
	void OnUpdate(Ocean::Timestep ts) override;
	void OnEvent(Ocean::Event& e) override;

private:
	Ocean::ShaderLibrary m_ShaderLibrary;
	Ocean::Ref<Ocean::Shader> m_Shader;

	Ocean::Ref<Ocean::VertexArray> m_VertexArray;

	Ocean::Ref<Ocean::Shader> m_FlatColorShader;
	Ocean::Ref<Ocean::VertexArray> m_SquareVA;

	Ocean::Ref<Ocean::Texture2D> m_Texture;
	Ocean::Ref<Ocean::Texture2D> m_LogoTexture;

	Ocean::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};
