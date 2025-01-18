#include "Camera.hpp"

// libs
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Ocean {
    
    OrthoCamera::OrthoCamera(f32 l, f32 r, f32 b, f32 t, f32 c, f32 d)
        : m_ViewProjectionMatrix(), m_ProjectionMatrix(glm::ortho(l, r, b, t, c, d)), m_ViewMatrix(1.0f)
    {
        this->m_ViewProjectionMatrix = this->m_ProjectionMatrix * this->m_ViewMatrix;
    }

    OrthoCamera::OrthoCamera(f32 aspectRatio, f32 c, f32 d)
        : m_ViewProjectionMatrix(), m_ProjectionMatrix(glm::ortho(-aspectRatio, aspectRatio,-1.0f, 1.0f, c, d)), m_ViewMatrix(1.0f)
    {
        this->m_ViewProjectionMatrix = this->m_ProjectionMatrix * this->m_ViewMatrix;
    }

    void OrthoCamera::SetProjection(f32 l, f32 r, f32 b, f32 t, f32 c, f32 d) {
        this->m_ProjectionMatrix = glm::ortho(l, r, b, t, c, d);

        this->m_ViewProjectionMatrix = this->m_ProjectionMatrix * this->m_ViewMatrix;
    }

}   // Ocean
