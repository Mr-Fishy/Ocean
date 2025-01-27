#include "gl_GraphicsContext.hpp"

#include "Ocean/Types/Integers.hpp"

#include "Ocean/Primitives/Assert.hpp"
#include "Ocean/Primitives/Log.hpp"

// libs
#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace Ocean {

    namespace Splash {
    
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
            oprint("\tVendor: %s\n", glGetString(GL_VENDOR));
            oprint("\tRenderer: %s\n", glGetString(GL_RENDERER));
            oprint("\tVersion: %s\n", glGetString(GL_VERSION));
        }
        
        void glGraphicsContext::SwapBuffers() {
            glfwSwapBuffers(this->p_WindowHandle);
        }

    }   // Splash

}   // Ocean
