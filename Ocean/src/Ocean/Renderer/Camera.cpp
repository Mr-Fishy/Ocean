
#include "ocpch.hpp"

#include "Camera.hpp"

//libs
#include <glm/gtc/matrix_transform.hpp>

namespace Ocean {

	OrthographicCamera::OrthographicCamera(
		float left, float right, float top, float bottom, float close, float dist)
		: m_ViewMatrix(1.0f) {
		// Using Raw Calculation (Almost Exactly GLM Ortho) To Get Proper Orientation
		m_ProjectionMatrix = glm::mat4{ 1.0f };
		m_ProjectionMatrix[0][0] = 2.f / (right - left);
		m_ProjectionMatrix[1][1] = 2.f / (bottom - top);
		m_ProjectionMatrix[2][2] = 1.f / (dist - close);
		m_ProjectionMatrix[3][0] = -(right + left) / (right - left);
		m_ProjectionMatrix[3][1] = -(bottom + top) / (bottom - top); // GLM Has: - (bottom + top) / (top - bottom);
		m_ProjectionMatrix[3][2] = -close / (dist - close);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix() {
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}	// Ocean