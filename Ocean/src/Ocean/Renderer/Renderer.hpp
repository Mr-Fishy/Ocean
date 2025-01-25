#pragma once

/**
 * @file Renderer.hpp
 * @author Evan F.
 * @brief The Ocean Renderer header file.
 * @date 01-14-2025
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Macros.hpp"

// libs
#include <glm/glm.hpp>

namespace Ocean {

    class Camera;

    namespace Shrimp {

        class VertexArray;
        class Shader;

    }   // Shrimp

    /**
     * @brief The overall renderer of Ocean. 
     * 
     * @details The functionality here is to be able to complete simple and higher level tasks. Such as resizing the viewport. 
     */
    class Renderer {
    private:
        /**
         * @brief The internal scene data for the Ocean Renderer.
         */
        struct SceneData {
            glm::mat4 viewProjectionMatrix; /** @brief The view-projection-matrix of the Camera that was last used. */

        };  // SceneData

    public:
        /**
         * @brief Initializes the Ocean Renderer for use.
         *
         * @details AKA the RenderCommand and the Renderer2D,
         */
        OC_STATIC void Init();
        /**
         * @brief Cleans up the Ocean Renderer to shutdown.
         * 
         * @details AKA the Renderer2D.
         */
        OC_STATIC void Shutdown();

        /**
         * @brief Begins a new overall scene.
         * 
         * @param camera The Camera to use.
         */
        OC_STATIC void BeginScene(Camera& camera);
        /**
         * @brief Ends an overall scene.
         */
        OC_STATIC void EndScene();

        /**
         * @brief Submit data to the Renderer to draw on screen.
         * 
         * @param shader The shader to use when drawing.
         * @param array The VertexArray of data to draw with.
         * @param transform The camera transform. (OPTIONAL)
         */
        OC_STATIC void Submit(const Ref<Shrimp::Shader>& shader, const Ref<Shrimp::VertexArray>& array, glm::mat4 transform = glm::mat4(1.0f));

        /**
         * @brief Call for when the Window is resized.
         * 
         * @param width The new width of the Window.
         * @param height The new height of the Window.
         */
        OC_STATIC void OnWindowResize(u32 width, u32 height);

    private:
        OC_STATIC_INLINE Scope<SceneData> s_SceneData = MakeScope<SceneData>(); /** @brief The static SceneData for the Renderer. */

    };  // Renderer

}   // Ocean
