#include "VertexArray.hpp"

#include "Ocean/Primitives/Assert.hpp"

#include "Ocean/Renderer/RendererAPI.hpp"
#include "Ocean/Renderer/OpenGL/gl_VertexArray.hpp"

namespace Ocean {

    namespace Splash {

        Ref<VertexArray> VertexArray::Create() {
            switch (RendererAPI::GetAPI()) {
                case RendererAPI::None:
                    break;

                case RendererAPI::OpenGL:
                    return MakeRef<glVertexArray>();

                case RendererAPI::Vulkan:
                break;
            }

            OASSERTM(false, "Unkown RendererAPI!");
            return nullptr;
        }

    }   // Splash

}   // Ocean