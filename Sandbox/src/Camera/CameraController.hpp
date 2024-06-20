#pragma once

#include <Ocean/Ocean.hpp>

class AppCamera : public Ocean::OrthographicCamera {
public:
	AppCamera(f32 aspectRatio, b8 rotation = false);

	void OnUpdate(Ocean::Timestep ts);

	void OnEvent(Ocean::Event& e);

	void OnResize(f32 width, f32 height);

	f32 GetZoomLevel() const { return m_ZoomLevel; }
	void SetZoomLevel(f32 level) { m_ZoomLevel = level; }

private:
	b8 OnWindowResized(Ocean::WindowResizeEvent& e);

	/* --- */

	f32 m_AspectRatio;
	f32 m_ZoomLevel;

	b8 m_RotationEnabled;
};
