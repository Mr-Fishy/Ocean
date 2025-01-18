#pragma once

/**
 * @file Renderer2D.hpp
 * @author Evan F.
 * @brief The Ocean Renderer2D header file.
 * @date 01-14-2025
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Types/FloatingPoints.hpp"
#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Renderer/Texture.hpp"

// libs
#include <glm/glm.hpp>

namespace Ocean {

    class Camera;

    /**
     * @brief A class of static functions to interact with Ocean's 2D renderer.
     */
    class Renderer2D {
    public:
        /**
         * @brief Initializes the 2D renderer for use.
         */
        static void Init();
        /**
         * @brief Cleans up the 2D renderer to shutdown.
         */
        static void Shutdown();

        /**
         * @brief Begins a 2D scene with the given Camera.
         * 
         * @param camera The Camera to use for rendering in the viewport.
         */
        static void BeginScene(const Camera& camera);
        /**
         * @brief Ends the 2D scene and Flush's the renderer.
         */
        static void EndScene();
        /**
         * @brief Draw's all of the indexed quads in the Renderer2D.
         */
        static void Flush();

        static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color);

        static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Shrimp::Texture2D>& texture, f32 tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Shrimp::Texture2D>& texture, f32 tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

        static void DrawQuad(const glm::mat4& transform, const glm::vec4 color, i32 entityID = -1);
        static void DrawQuad(const glm::mat4& transform, const Ref<Shrimp::Texture2D>& texture, f32 tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), i32 entityID = -1);
        
        /**
         * @brief A struct to hold the stats of the Renderer2D.
         */
        struct Statistics {
            u32 drawCalls = 0; /** @brief The number of draw calls made in a frame. */
            u32 quadCount = 0; /** @brief The number of quads that were drawn in a frame. */

            /**
             * @brief Get the number of vertices that were drawn in a frame.
             * 
             * @return u32 
             */
            inline u32 GetVertexCount() const { return this->quadCount * 4; }
            /**
             * @brief Get the number of indices that were drawn in a frame.
             * 
             * @return u32 
             */
            inline u32 GetIndexCount() const { return this->quadCount * 6; }

        };  // Statistics

        /**
         * @brief Reset the Statistics. Primarily used in a new frame.
         */
        static void ResetStats();
        /**
         * @brief Get the Statistics from the Renderer2D.
         * 
         * @return Statistics 
         */
        static Statistics GetStats();

    private:
        /**
         * @brief Starts a batch of quad's to render.
         */
        static void StartBatch();
        /**
         * @brief Flushes the Renderer2D and starts the next batch.
         */
        static void NextBatch();

    };  // Renderer2D

}   // Ocean
