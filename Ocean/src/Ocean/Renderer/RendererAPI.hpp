#pragma once

/**
 * @file RendererAPI.hpp
 * @author Evan F.
 * @brief The abstract RendererAPI for Ocean.
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

    namespace Splash {

        class VertexArray;
        
        /**
         * @brief The RendererAPI controls the instance of the renderer API.
         */
        class RendererAPI {
        public:
            /**
             * @brief An enum of available renderer API's.
             */
            enum API {
                /** @brief Null protection option. */
                None = 0,

                /** @brief OpenGL renderer API. */
                OpenGL = 1,

                /** @brief Vulkan renderer API. */
                Vulkan = 2,

            };  // API

        public:
            RendererAPI() = default;
            virtual ~RendererAPI() = default;

            /** @copydoc RenderCommand::Init() */
            virtual void Init() = 0;

            /** @copydoc RenderCommand::SetViewport() */
            virtual void SetViewport(u32 x, u32 y, u32 w, u32 h) = 0;

            /** @copydoc RenderCommand::SetClearColor() */
            virtual void SetClearColor(const glm::vec4& color) = 0;
            /** @copydoc RenderCommand::Clear() */
            virtual void Clear() = 0;

            /** @copydoc RenderCommand::DrawIndexed() */
            virtual void DrawIndexed(const Ref<VertexArray>& array, u32 indexCount = 0) = 0;

            /**
             * @brief Get's the API that Ocean is set to.
             * 
             * @return API
             */
            OC_STATIC_INLINE API GetAPI() { return s_API; }

            /**
             * @brief Create's a new RendererAPI instance.
             * 
             * @return Scope<RendererAPI> 
             */
            OC_STATIC Scope<RendererAPI> Create();

        private:
            OC_NO_COPY(RendererAPI);

            OC_STATIC_INLINE API s_API = API::Vulkan; /** @brief The API to use for the renderer API in Ocean. */

        };  // RendererAPI

    }   // Splash

}   // Ocean
