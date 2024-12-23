#pragma once

// libs
#include <glm/glm.hpp>

namespace Ocean {

    class Camera {
    public:
        virtual ~Camera() = default;



        inline const glm::mat4& GetViewProjectionMatrix() const { return this->m_ViewProjectionMatrix; }

    protected:
        glm::mat4 m_ViewProjectionMatrix;

    };

}   // Ocean