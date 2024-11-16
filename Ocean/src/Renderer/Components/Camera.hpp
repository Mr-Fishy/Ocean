#pragma once

#include <glm/ext/vector_float3.hpp>

namespace Ocean {

    class Camera {
    public:
        enum CameraType {
            Orthographic = 0,
            Perspective = 1,
        };

        Camera(CameraType type, const glm::vec3& pos = { 0, 0, 0 }, const glm::vec3& rotation = { 0, 0, 0 });
        ~Camera();

    private:
        glm::vec3 m_Position;
        glm::vec3 m_Rotation;

    };

}   // Ocean