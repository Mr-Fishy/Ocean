#include "Sandbox2D.hpp"
#include "Ocean/Renderer/Renderer2D.hpp"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D") {

}

void Sandbox2D::OnAttach() {

}

void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnUpdate(Timestep ts) {
    Ocean::Renderer2D::ResetStats();
    {
        Ocean::RenderCommand::SetClearColor({ 0.5f, 0.1f, 0.1f, 1.0f });
        Ocean::RenderCommand::Clear();
    }
}