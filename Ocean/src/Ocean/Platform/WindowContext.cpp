#include "WindowContext.hpp"

#include "Ocean/Primitives/Exceptions.hpp"
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

    WindowContext::PlatformID WindowContext::GetPlatformID() {
        switch (glfwGetPlatform()) {
            case GLFW_PLATFORM_WIN32:
                return WindowContext::PlatformID::WINDOWS;

            case GLFW_PLATFORM_X11:
                return WindowContext::PlatformID::LINUX_X11;

            case GLFW_PLATFORM_WAYLAND:
                return WindowContext::PlatformID::LINUX_WAYLAND;

            case GLFW_PLATFORM_COCOA:
                return WindowContext::PlatformID::MACOS_COCOA;
        }

        throw Exception(Error::BAD_PLATFORM, "Could not find a supported window platform!");

        return WindowContext::PlatformID::NONE;
    }

}   // Ocean
