#pragma once

/**
 * @file gl_GraphicsContext.hpp
 * @author Evan F.
 * @brief The implementation of an OpenGL GraphicsContext.
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Primitives/Macros.hpp"

#include "Ocean/Renderer/GraphicsContext.hpp"

struct GLFWwindow;

namespace Ocean {

    namespace Splash {

        /**
         * @brief An OpenGL implemented GraphicsContext.
         */
        class glGraphicsContext : public GraphicsContext {
        public:
            glGraphicsContext() = delete;
            /** @copydoc GraphicsContext::GraphicsContext(GLFWwindow*) */
            glGraphicsContext(GLFWwindow* window);
            ~glGraphicsContext();

            /** @copydoc GraphicsContext::Init() */
            virtual void Init() override final;
            
            /** @copydoc GraphicsContext::SwapBuffers() */
            virtual void SwapBuffers() override final;

        private:
            OC_NO_COPY(glGraphicsContext);

        };  // GraphicsContext

    }   // Splash

}   // Ocean
