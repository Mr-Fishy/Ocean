#include "GameLayer.hpp"

Battleship::Battleship() : Layer("BattleShip"), m_Camera(1280.0f / 720.0f) { }

Battleship::~Battleship() { }

void Battleship::OnAttach() {
	m_CheckboardTexture = Ocean::Texture2D::Create("assets/textures/Checkerboard.png");
	m_BackgroundTexture = Ocean::Texture2D::Create("assets/textures/OceanTexture.jpg");
}

void Battleship::OnDetach() { }

void Battleship::OnUpdate(Ocean::Timestep ts) {
	m_Camera.OnUpdate(ts);

	Ocean::Renderer2D::ResetStats();

	Ocean::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Ocean::RenderCommand::Clear();

	Ocean::Renderer2D::BeginScene(m_Camera.Get());

	m_GameBoard.Draw();

	Ocean::Renderer2D::DrawQuad(ZeroPoint, WindowSize, m_BackgroundTexture, 10.0f);

	Ocean::Renderer2D::EndScene();
}

void Battleship::OnEvent(Ocean::Event& e) {
	m_Camera.OnEvent(e);
}
