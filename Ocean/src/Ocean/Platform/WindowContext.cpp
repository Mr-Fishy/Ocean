#include "WindowContext.hpp"

#include "Ocean/Types/Integers.hpp"

#include "Ocean/Primitives/Log.hpp"

// libs
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Ocean {

    static void GLFW_ErrorCallback(i32 error, cstring description) {
        oprint("GLWF Error (%i): %s\n", error, description);
    }



    void WindowContext::Init() {
        glfwSetErrorCallback(GLFW_ErrorCallback);

        /** @todo Make sure glfw is initialized before the renderer, as glfw gets platform information that may be required at renderer initialization. */
        if (!glfwInit()) {
            oprint("GLFW Init Error!\n");
            glfwTerminate();

            return;
        }
    }

    void WindowContext::Shutdown() {
        glfwTerminate();
    }

}   // Ocean
