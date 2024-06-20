#include "ocpch.hpp"

#include "Ocean/Renderer/Camera.hpp"

#include <Core/Log.hpp>

// libs
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Ocean {

	void Camera::UpdateView() {
		quat orientation = GetOrientation();

		m_ViewMatrix = glm::inverse(
			glm::translate(mat4(1.0f), m_Position) * glm::toMat4(orientation)
		);
	}

	quat Camera::GetOrientation() const {
		return quat(vec3(-m_Rotation.x, -m_Rotation.y, 0.0f));
	}



	OrthographicCamera::OrthographicCamera(f32 left, f32 right, f32 bottom, f32 top, f32 close, f32 dist) : Camera() { }

	void OrthographicCamera::SetProjection(f32 left, f32 right, f32 bottom, f32 top, f32 close, f32 dist) {
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, close, dist);

		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix() {
		m_ViewMatrix = glm::inverse(
			glm::translate(mat4(1.0f), m_Position) * glm::rotate(mat4(1.0f), glm::radians(m_Rotation.x), vec3(0, 0, 1))
		);

		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}	// Ocean
