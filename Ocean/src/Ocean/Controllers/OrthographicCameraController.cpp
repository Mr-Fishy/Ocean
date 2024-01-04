
#include "ocpch.hpp"
#include "OrthographicCameraController.hpp"

#include "Ocean/Core/Input.hpp"
#include "Ocean/Core/KeyCodes.hpp"

namespace Ocean {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_RotationEnabled(rotation),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel) {}

	void OrthographicCameraController::OnUpdate(Timestep ts) {
		OC_PROFILE_FUNCTION();

		// Switch Statement Control Would Be Ideal But Needs A "GetKeyPressed" Type Function Which Is An Event Nightmare And Wouldn't Be Based On "OnUpdate" Timing
		if (Input::IsKeyPressed(OC_KEY_A)) {
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(OC_KEY_D)) {
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}

		if (Input::IsKeyPressed(OC_KEY_S)) {
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(OC_KEY_W)) {
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}

		if (m_RotationEnabled) {
			if (Input::IsKeyPressed(OC_KEY_Q)) {
				m_CameraRotation += m_CameraRotationSpeed * ts;
			}

			if (Input::IsKeyPressed(OC_KEY_E)) {
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			}

			if (m_CameraRotation > 180.0f) {
				m_CameraRotation -= 360.0f;
			}
			else if (m_CameraRotation <= -180.0f) {
				m_CameraRotation += 360.0f;
			}

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e) {
		OC_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<MouseScrolledEvent>
			(OC_BIND_EVENT_FN(OrthographicCameraController::OnsMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>
			(OC_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::OnResize(float width, float height) {
		m_AspectRatio = width / height;

		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	bool OrthographicCameraController::OnsMouseScrolled(MouseScrolledEvent& e) {
		OC_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);

		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e) {
		OC_PROFILE_FUNCTION();

		OnResize((float)e.GetWidth(), (float)e.GetHeight());

		return false;
	}

}	// Ocean
