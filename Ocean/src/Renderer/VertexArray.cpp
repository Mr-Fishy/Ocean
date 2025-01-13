#include "VertexArray.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"

#include "Renderer/RendererAPI.hpp"
#include "Renderer/OpenGL/gl_VertexArray.hpp"

namespace Ocean {

    namespace Shrimp {

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

    }   // Shrimp

}   // Ocean