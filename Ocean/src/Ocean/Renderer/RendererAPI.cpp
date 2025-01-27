#include "RendererAPI.hpp"

#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Assert.hpp"

#include "Ocean/Renderer/OpenGL/gl_RendererAPI.hpp"

namespace Ocean {

    namespace Splash {
    
        Scope<RendererAPI> RendererAPI::Create() {
            switch (s_API) {
                case None:
                    break;

                case OpenGL:
                    return MakeScope<glRendererAPI>();

                case Vulkan:
                    return nullptr;
            }

            OASSERTM(false, "Unkown Rendering API!");
            return nullptr;
        }

    }   // Splash

}   // Ocean
