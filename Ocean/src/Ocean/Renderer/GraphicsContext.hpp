#pragma once

/**
 * @file GraphicsContext.hpp
 * @author Evan F.
 * @brief The abstract GraphicsContext for Ocean.
 * @date 01-14-2025
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Macros.hpp"

struct GLFWwindow;

namespace Ocean {

    namespace Splash {
    
        /**
         * @brief The GraphicsContext controls the instance of the renderer's window context.
         */
        class GraphicsContext {
        public:
            /**
             * @brief Construct a new graphics context for the given window.
             * 
             * @param window The window to attach the graphics context to.
             */
            GraphicsContext(GLFWwindow* window);
            virtual ~GraphicsContext() = default;

            /**
             * @brief Initialize the GraphicsContext.
             * 
             * @details AKA the rendering API.
             */
            virtual void Init() = 0;
            
            /**
             * @brief Swap the framebuffers of the Window that owns the context.
             */
            virtual void SwapBuffers() = 0;

            /**
             * @brief Create's a GraphicsContext attached to the given Window handle.
             * 
             * @param windowHandle The Window platform handle.
             * @return Scope<GraphicsContext> 
             */
            OC_STATIC Scope<GraphicsContext> Create(void* windowHandle);

        private:
            OC_NO_COPY(GraphicsContext);

        protected:
            GLFWwindow* p_WindowHandle;

        };  // GraphicsContext

    }   // Splash

}   // Ocean
