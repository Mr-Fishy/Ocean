#pragma once

#include "Renderer/GraphicsContext.hpp"

struct GLFWwindow;

namespace Ocean {

    namespace Shrimp {
    
        class vkGraphicsContext : public GraphicsContext {
        public:
            vkGraphicsContext(GLFWwindow* window);
            virtual ~vkGraphicsContext();

            virtual void Init() override final;
            
            virtual void SwapBuffers() override final;

        private:
            GLFWwindow* p_WindowHandle;

        };  // GraphicsContext

    }   // Shrimp

}   // Ocean
