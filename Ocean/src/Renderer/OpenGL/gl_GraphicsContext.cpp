#include "gl_GraphicsContext.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"
#include "Ocean/Core/Primitives/Log.hpp"
#include "Ocean/Core/Types/Integers.hpp"

// libs
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

namespace Ocean {

    namespace Shrimp {
    
        glGraphicsContext::glGraphicsContext(GLFWwindow* window) : p_WindowHandle(window) {
            OASSERTM(this->p_WindowHandle != nullptr, "Window Handle Is A nullptr!");
        }

        glGraphicsContext::~glGraphicsContext() {

        }

        void glGraphicsContext::Init() {
            glfwMakeContextCurrent(this->p_WindowHandle);

            i32 status = gladLoadGL(static_cast<GLADloadfunc>(glfwGetProcAddress));
            OASSERTM(status, "Failed to initialize Glad!");

            oprint("OpenGL Info\n");
            oprint("\tVendor: %s", glGetString(GL_VENDOR));
            oprint("\tRenderer: %s", glGetString(GL_RENDERER));
            oprint("\tVersion: %s", glGetString(GL_VERSION));
        }
        
        void glGraphicsContext::SwapBuffers() {
            glfwSwapBuffers(this->p_WindowHandle);
        }

    }   // Shrimp

}   // Ocean
