
#include "ocpch.hpp"
#include "EditorCamera.h"

#include "Ocean/Core/Input.hpp"
#include "Ocean/Core/KeyCodes.hpp"
#include "Ocean/Core/MouseCodes.hpp"

// libs
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Ocean {

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip),
			Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip)) {
		UpdateView();
	}

	void EditorCamera::OnUpdate(Timestep ts) {
		glm::vec2 delta = GetDelta();

		if (Input::IsKeyPressed(Key::LeftShift)) {
			if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
				MousePan(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
				MouseZoom(delta.y);

			// Keyboard Movement //
			if (Input::IsKeyPressed(Key::LeftControl)) {
				if (Input::IsKeyPressed(Key::Up))
					ArrowZoom(ArrowDirection::UP);
				else if (Input::IsKeyPressed(Key::Down))
					ArrowZoom(ArrowDirection::DOWN);
			}
			else {
				if (Input::IsKeyPressed(Key::Left))
					ArrowPan(ArrowDirection::LEFT);
				else if (Input::IsKeyPressed(Key::Right))
					ArrowPan(ArrowDirection::RIGHT);

				if (Input::IsKeyPressed(Key::Up))
					ArrowPan(ArrowDirection::UP);
				else if (Input::IsKeyPressed(Key::Down))
					ArrowPan(ArrowDirection::DOWN);
			}
		}
		else if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle)) {
			MouseRotate(delta);
		}
		else {
			// Keyboard Movement//
			if (Input::IsKeyPressed(Key::Left))
				ArrowRotate(ArrowDirection::LEFT);
			else if (Input::IsKeyPressed(Key::Right))
				ArrowRotate(ArrowDirection::RIGHT);

			if (Input::IsKeyPressed(Key::Up))
				ArrowRotate(ArrowDirection::UP);
			else if (Input::IsKeyPressed(Key::Down))
				ArrowRotate(ArrowDirection::DOWN);
		}

		UpdateView();
	}

	void EditorCamera::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(OC_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}

	glm::vec3 EditorCamera::GetUpVector() const {
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetRightVector() const {
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetForwardVector() const {
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::quat EditorCamera::GetOrientation() const {
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}

	void EditorCamera::UpdateProjection() {
		m_AspectRatio = m_Viewport.x / m_Viewport.y;
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::UpdateView() {
		// m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation
		m_Position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e) {
		float delta = e.GetYOffset() * 0.1f;

		MouseZoom(delta);
		UpdateView();

		return false;
	}

	void EditorCamera::MousePan(glm::vec2 delta) {
		auto [xSpeed, ySpeed] = PanSpeed();

		m_FocalPoint += -GetRightVector() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += GetUpVector() * delta.y * ySpeed * m_Distance;
	}

	void EditorCamera::MouseRotate(glm::vec2 delta) {
		float yawSign = GetUpVector().y < 0 ? -1.0f : 1.0f;

		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float delta) {
		m_Distance -= delta * ZoomSpeed();

		if (m_Distance < 1.0f) {
			m_FocalPoint += GetForwardVector();
			m_Distance = 1.0f;
		}
	}

	void EditorCamera::ArrowPan(ArrowDirection direction) {
		auto [xSpeed, ySpeed] = PanSpeed();

		switch (direction) {
			case Ocean::EditorCamera::LEFT:
				m_FocalPoint += -GetRightVector() * 0.06f * xSpeed * m_Distance;
				break;

			case Ocean::EditorCamera::RIGHT:
				m_FocalPoint += GetRightVector() * 0.06f * xSpeed * m_Distance;
				break;
		}

		switch (direction) {
			case Ocean::EditorCamera::UP:
				m_FocalPoint += GetUpVector() * 0.06f * ySpeed * m_Distance;
				break;

			case Ocean::EditorCamera::DOWN:
				m_FocalPoint += -GetUpVector() * 0.06f * ySpeed * m_Distance;
				break;
		}
	}

	void EditorCamera::ArrowRotate(ArrowDirection direction) {
		float yawSign = GetUpVector().y < 0 ? -1.0f : 1.0f;

		switch (direction) {
			case Ocean::EditorCamera::LEFT:
				m_Yaw += yawSign * 0.06f * RotationSpeed();
				break;

			case Ocean::EditorCamera::RIGHT:
				m_Yaw += yawSign * -0.06f * RotationSpeed();
				break;
		}

		switch (direction) {
			case Ocean::EditorCamera::UP:
				m_Pitch += 0.06f * RotationSpeed();
				break;

			case Ocean::EditorCamera::DOWN:
				m_Pitch += -0.06f * RotationSpeed();
				break;
		}
	}

	void EditorCamera::ArrowZoom(ArrowDirection direction) {
		switch (direction) {
			case Ocean::EditorCamera::UP:
				m_Distance -= 0.06f * ZoomSpeed();
				break;

			case Ocean::EditorCamera::DOWN:
				m_Distance -= -0.06f * ZoomSpeed();
				break;

		}

		if (m_Distance < 1.0f) {
			m_FocalPoint += GetForwardVector();
			m_Distance = 1.0f;
		}
	}

	const glm::vec2 EditorCamera::GetDelta() {
		const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
		glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;

		m_InitialMousePosition = mouse;

		return delta;
	}

	glm::vec3 EditorCamera::CalculatePosition() const {
		return m_FocalPoint - GetForwardVector() * m_Distance;
	}

	std::pair<float, float> EditorCamera::PanSpeed() const {
		float x = std::min(m_Viewport.x / 1000.0f, 2.4f); // Max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f; // Magic Numbers From The Cherno

		float y = std::min(m_Viewport.y / 1000.0f, 2.4f); // Max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f; // Magic Numbers From The Cherno

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const {
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const {
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);

		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100

		return speed;
	}

}	// Ocean
