#pragma once

#include "Ocean/Core/Types.hpp"

namespace Ocean {

	class Camera {
	public:
		Camera() : m_ViewMatrix(1.0f) { }
		virtual ~Camera() = default;

		const mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const mat4& GetProjectionViewMatrix() const { return m_ProjectionViewMatrix; }

		// This theoretically allows 3D space movement :)
		virtual void UpdateView();

	protected:
		quat GetOrientation() const;

		virtual void RecalculateViewMatrix() = 0;

		/* --- */

		mat4 m_ProjectionMatrix{};
		mat4 m_ViewMatrix{};
		mat4 m_ProjectionViewMatrix{};

		vec3 m_Position{ 0.0f, 0.0f, 0.0f };
		vec2 m_Rotation{ 0.0f, 0.0f };
	};



	class OrthographicCamera : public Camera {
	public:
		OrthographicCamera() = default;
		OrthographicCamera(f32 left, f32 right, f32 bottom, f32 top, f32 close = -1.0f, f32 dist = 1.0f);
		
		void SetProjection(f32 left, f32 right, f32 bottom, f32 top, f32 close = -1.0f, f32 dist = 1.0f);

		OrthographicCamera& Get() { return *this; }

	protected:
		virtual void RecalculateViewMatrix() override;
	};

}	// Ocean
