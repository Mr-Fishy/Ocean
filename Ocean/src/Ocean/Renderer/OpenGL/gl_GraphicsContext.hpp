#pragma once

/**
 * @file gl_GraphicsContext.hpp
 * @author Evan F.
 * @brief The implementation of an OpenGL GraphicsContext.
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Renderer/GraphicsContext.hpp"

struct GLFWwindow;

namespace Ocean {

    namespace Splash {

        class glGraphicsContext : public GraphicsContext {
        public:
            glGraphicsContext() = delete;
            glGraphicsContext(GLFWwindow* window);
            glGraphicsContext(const glGraphicsContext&) = delete;
            virtual ~glGraphicsContext();

            glGraphicsContext& operator = (const glGraphicsContext&) = delete;

            virtual void Init() override final;
            
            virtual void SwapBuffers() override final;

        private:
            GLFWwindow* p_WindowHandle;

        };  // GraphicsContext

    }   // Splash

}   // Ocean
