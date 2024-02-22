#pragma once

#include "Camera.hpp"
#include "Ocean/Core/Timestep.hpp"
#include "Ocean/Events/Event.hpp"
#include "Ocean/Events/MouseEvent.hpp"

// libs
#include <glm/glm.hpp>

namespace Ocean {

	class EditorCamera : public Camera {
	public:
		EditorCamera() = default;
		// Constructs a perspective editor camera for the editor.
		// @param fov: The Field Of View for the camera.
		// @param aspectRatio: The viewport aspect ratio for the camera.
		// @param nearClip: The near clipping distance.
		// @param farClip: The far clipping distance.
		//
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip); // Perspective Projection Camera
		// EditorCamera(float left, float right, float bottom, float top, float close, float dist); // TODO: Orthographic Projection Camera (Switch Between Projection Like Blender)

		// The EditorCamera update call.
		// @param ts: The engine timestep.
		//
		void OnUpdate(Timestep ts);
		// The EditorCamera event call.
		// @param e: The event to handle.
		//
		void OnEvent(Event& e);

		// Gets the distance between the camera and the focal point.
		// @returns float
		//
		inline float GetDistance() const { return m_Distance; }
		// Sets the distance between the camera and the focal point.
		// @param distance: The new distance between the camera and the focal point.
		//
		inline void SetDistance(float distance) { m_Distance = distance; }

		// Sets the EditorCamera viewport size.
		// @param width: The viewport width.
		// @param height: The viewport height.
		//
		inline void SetViewportSize(float width, float height) { m_Viewport = { width, height }; UpdateProjection(); }

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

		glm::vec3 GetUpVector() const;
		glm::vec3 GetRightVector() const;
		glm::vec3 GetForwardVector() const;

		const glm::vec3 GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }

	private:
		enum ArrowDirection {
			LEFT = 1,
			UP = 2,
			RIGHT = 3,
			DOWN = 4
		};

		/* --- */

		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(glm::vec2 delta);
		void MouseRotate(glm::vec2 delta);
		void MouseZoom(float delta);

		void ArrowPan(ArrowDirection direction);
		void ArrowRotate(ArrowDirection direction);
		void ArrowZoom(ArrowDirection direction);

		const glm::vec2 GetDelta();

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

		/* --- */

		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		glm::mat4 m_ViewMatrix{ };
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		float m_Distance = 10.0f;
		float m_Pitch = 0.0f, m_Yaw = 0.0f;

		glm::vec2 m_Viewport = { 1280.0f, 720.0f };
	};

}	// Ocean
