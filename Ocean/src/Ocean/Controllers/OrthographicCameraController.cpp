
#include "ocpch.hpp"

#include "OrthographicCameraController.hpp"

#include "Ocean/Core/Input.hpp"
#include "Ocean/Core/KeyCodes.hpp"

namespace Ocean {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_RotationEnabled(rotation),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel) {}

	void OrthographicCameraController::OnUpdate(Timestep ts) {
		// TODO: Switch Statements Are Faster, Make Functionality Switch Statement Ideal

		if (Input::IsKeyPressed(OC_KEY_A)) {
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(OC_KEY_D)) {
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;
		}

		if (Input::IsKeyPressed(OC_KEY_S)) {
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(OC_KEY_W)) {
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		}

		if (m_RotationEnabled) {
			if (Input::IsKeyPressed(OC_KEY_Q)) {
				m_CameraRotation += m_CameraRotationSpeed * ts;
			}
			else if (Input::IsKeyPressed(OC_KEY_E)) {
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			}

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<MouseScrolledEvent>
			(OC_BIND_EVENT_FN(OrthographicCameraController::OnsMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>
			(OC_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnsMouseScrolled(MouseScrolledEvent& e) {
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);

		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e) {
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();

		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

}	// Ocean
