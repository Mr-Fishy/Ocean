#include "RendererAPI.hpp"

#include "Ocean/Core/Types/SmartPtrs.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"

#include "Renderer/OpenGL/gl_RendererAPI.hpp"

namespace Ocean {

    namespace Shrimp {
    
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

    }   // Shrimp

}   // Ocean
