
#include "ocpch.hpp"
#include "OrthoCamera.hpp"

//libs
#include <glm/gtc/matrix_transform.hpp>

namespace Ocean {

	OrthographicCamera::OrthographicCamera(
		float left, float right, float top, float bottom, float close, float dist)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, close, dist)), m_ViewMatrix(1.0f) {
		OC_PROFILE_FUNCTION();

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(
		float left, float right, float bottom, float top, float close, float dist) {
		OC_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, close, dist);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix() {
		OC_PROFILE_FUNCTION();
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}	// Ocean
