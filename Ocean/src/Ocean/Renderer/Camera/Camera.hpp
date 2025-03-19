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

    /**
     * @brief An enum of the possible camera types. 
     */
    enum CameraType {
        /** @brief Null protection option. */
        None = 0,

        /** @brief Orthographic Camera, often for 2D drawing. */
        Orthographic = 1,
        /** @brief Perspective Camera, often for 3D drawing. */
        Perspective = 2,

    }; // CameraType

    /**
     * @brief A base camera to view 2D or 3D space.
     */
    class Camera {
    public:
        virtual ~Camera() = default;

        /**
         * @brief Get the view projection matrix from the camera.
         * 
         * @return const glm::mat4& 
         */
        virtual const glm::mat4& GetViewProjectionMatrix() const = 0;
        /**
         * @brief Get the projection matrix from the camera.
         * 
         * @return const glm::mat4& 
         */
        virtual const glm::mat4& GetProjectionMatrix() const = 0;
        /**
         * @brief Get the view matrix from the camera.
         * 
         * @return const glm::mat4& 
         */
        virtual const glm::mat4& GetViewMatrix() const = 0;

        /**
         * @brief Get the CameraType.
         * 
         * @return CameraType 
         */
        virtual CameraType GetType() const { return CameraType::None; }

    };  // Camera

    /**
     * @brief An orthographic camera that can be used for 2D or 3D spaces.
     */
    class OrthoCamera : public Camera {
    public:
        /**
         * @brief Construct a new orthographic camera.
         * 
         * @param l Left edge.
         * @param r Right edge.
         * @param b Bottom edge.
         * @param t Top edge.
         * @param c Close clipping distance.
         * @param d Distant clipping distance.
         */
        OrthoCamera(f32 l, f32 r, f32 b, f32 t, f32 c = -1.0f, f32 d = 1.0f);
        /**
         * @brief Construct a new orthographic camera.
         * 
         * @param aspectRatio The aspect ratio of the view.
         * @param c Close clipping distance.
         * @param d Distant clipping distance.
         */
        OrthoCamera(f32 aspectRatio, f32 c = -1.0f, f32 d = 1.0f);
        virtual ~OrthoCamera() = default;

        /**
         * @brief Set a new projection for the camera.
         * 
         * @copydetails OrthoCamera::OrthoCamera(f32, f32, f32, f32, f32, f32)
         */
        void SetProjection(f32 l, f32 r, f32 b, f32 t, f32 c = -1.0f, f32 d = 1.0f);

        /** @copydoc Camera::GetViewProjectionMatrix() */
        virtual const glm::mat4& GetViewProjectionMatrix() const override { return this->m_ViewProjectionMatrix; }
        /** @copydoc Camera::GetProjectionMatrix() */
        virtual const glm::mat4& GetProjectionMatrix() const override { return this->m_ProjectionMatrix; }
        /** @copydoc Camera::GetViewMatrix() */
        virtual const glm::mat4& GetViewMatrix() const override { return this->m_ViewMatrix; }

        /** @copydoc Camera::GetType() */
        virtual CameraType GetType() const override { return CameraType::Orthographic; }

    private:
		glm::mat4 m_ViewProjectionMatrix; /** @brief The view-projection-matrix of the camera. */
        glm::mat4 m_ProjectionMatrix; /** @brief The projection-matrix of the camera. */
		glm::mat4 m_ViewMatrix; /** @brief The view-matrix of the camera. */
    
    };  // OrthoCamera

}   // Ocean
