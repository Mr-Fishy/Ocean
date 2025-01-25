#pragma once

/**
 * @file Camera.hpp
 * @author Evan F.
 * @brief The Renderer Camera's of Ocean.
 * @date 01-15-2025
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Types/FloatingPoints.hpp"

// libs
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

namespace Ocean {

    enum CameraType {
        /** @brief Null protection option. */
        None = 0,

        /** @brief Orthographic Camera, often for 2D drawing. */
        Orthographic = 1,
        /** @brief Perspective Camera, often for 3D drawing. */
        Perspective = 2,

    }; // CameraType

    class Camera {
    public:
        virtual ~Camera() = default;

        virtual const glm::mat4& GetViewProjectionMatrix() const = 0;
        virtual const glm::mat4& GetProjectionMatrix() const = 0;
        virtual const glm::mat4& GetViewMatrix() const = 0;

        virtual CameraType GetType() const { return CameraType::None; }

    };  // Camera

    class OrthoCamera : public Camera {
    public:
        OrthoCamera(f32 l, f32 r, f32 b, f32 t, f32 c = -1.0f, f32 d = 1.0f);
        OrthoCamera(f32 aspectRatio, f32 c = -1.0f, f32 d = 1.0f);
        virtual ~OrthoCamera() = default;

        void SetProjection(f32 l, f32 r, f32 b, f32 t, f32 c = -1.0f, f32 d = 1.0f);

        virtual const glm::mat4& GetViewProjectionMatrix() const override { return this->m_ViewProjectionMatrix; }
        virtual const glm::mat4& GetProjectionMatrix() const override { return this->m_ProjectionMatrix; }
        virtual const glm::mat4& GetViewMatrix() const override { return this->m_ViewMatrix; }

        virtual CameraType GetType() const override { return CameraType::Orthographic; }

    private:
		glm::mat4 m_ViewProjectionMatrix;
        glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
    
    };  // OrthoCamera

}   // Ocean
