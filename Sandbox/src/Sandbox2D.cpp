#include "Sandbox2D.hpp"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_Camera(0.0f, 1200.0f, 800.0f, 0.0f) {

}

void Sandbox2D::OnAttach() {

}

void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnUpdate(Timestep ts) {
    Ocean::Renderer2D::ResetStats();
    {
        Ocean::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        Ocean::RenderCommand::Clear();
    }

    Ocean::Renderer2D::BeginScene(this->m_Camera);

    Ocean::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 100.0f, 100.0f }, { 1.0f, 0.0f, 0.0f, 1.0f });
    Ocean::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });

    Ocean::Renderer2D::EndScene();
}
