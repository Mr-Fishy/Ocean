#pragma once

#include "Renderer/GraphicsContext.hpp"

struct GLFWwindow;

namespace Ocean {

    namespace Shrimp {
    
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

    }   // Shrimp

}   // Ocean
