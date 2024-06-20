#include "CameraController.hpp"

AppCamera::AppCamera(f32 aspectRatio, b8 rotation)
	: m_AspectRatio(aspectRatio), m_RotationEnabled(rotation), m_ZoomLevel(1.0f) {
	SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
}

void AppCamera::OnUpdate(Ocean::Timestep ts) {
	// RecalculateViewMatrix();
}

void AppCamera::OnEvent(Ocean::Event& e) {
	Ocean::EventDispatcher dispatcher(e);

	dispatcher.Dispatch<Ocean::WindowResizeEvent>
		(OC_BIND_EVENT_FN(AppCamera::OnWindowResized));
}

void AppCamera::OnResize(f32 width, f32 height) {
	m_AspectRatio = width / height;

	SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
}

b8 AppCamera::OnWindowResized(Ocean::WindowResizeEvent& e) {
	OnResize(static_cast<f32>(e.GetWidth()), static_cast<f32>(e.GetHeight()));

	return false;
}
