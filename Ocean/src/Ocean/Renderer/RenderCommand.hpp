#pragma once

/**
 * @file RenderCommand.hpp
 * @author Evan F.
 * @brief The Ocean RenderCommand header file.
 * @date 01-14-2025
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Macros.hpp"

#include "Ocean/Renderer/RendererAPI.hpp"
#include "Ocean/Renderer/VertexArray.hpp"

// libs
#include <glm/glm.hpp>

namespace Ocean {

    /**
     * @brief A class of static functions to interact with the RendererAPI from Ocean.
     */
    class RenderCommand {
    public:
        /**
         * @brief Initializes the RendererAPI.
         */
        OC_STATIC_INLINE void Init() {
            s_RendererAPI->Init();
        }

        /**
         * @brief Set the viewport origin and size with the given data.
         * 
         * @param x The x-position of the viewport.
         * @param y The y-position of the viewport.
         * @param w The width of the viewport.
         * @param h The height of the viewport.
         */
        OC_STATIC_INLINE void SetViewport(u32 x, u32 y, u32 w, u32 h) {
            s_RendererAPI->SetViewport(x, y, w, h);
        }
        /**
         * @brief Set the clear color of the viewport.
         * 
         * @details AKA the background color of the window.
         * 
         * @param color The color to use.
         */
        OC_STATIC_INLINE void SetClearColor(const glm::vec4& color) {
            s_RendererAPI->SetClearColor(color);
        }

        /**
         * @brief Clears the viewport of previously drawn data.
         */
        OC_STATIC_INLINE void Clear() {
            s_RendererAPI->Clear();
        }

        /**
         * @brief Draw the index data from the given VertexArray.
         * 
         * @param array The VertexArray to draw from.
         * @param count The number of indices to draw. (OPTIONAL)
         */
        OC_STATIC_INLINE void DrawIndexed(const Ref<Splash::VertexArray>& array, u32 count = 0) {
            s_RendererAPI->DrawIndexed(array, count);
        }

        OC_STATIC_INLINE Scope<Splash::RendererAPI>& GetRenderer() {
            return s_RendererAPI;
        }

    private:
        OC_STATIC_INLINE Scope<Splash::RendererAPI> s_RendererAPI = Splash::RendererAPI::Create(); /** @brief The static instance of the RendererAPI. */

    };  // RenderCommand

}   // Ocean
