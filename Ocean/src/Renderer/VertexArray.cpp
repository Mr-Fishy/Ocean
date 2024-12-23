#include "VertexArray.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"

#include "Renderer/RendererAPI.hpp"
#include "Renderer/OpenGL/gl_VertexArray.hpp"

namespace Ocean {

    namespace Shrimp {

        SharedPtr<VertexArray> VertexArray::Create() {
            switch (RendererAPI::GetAPI()) {
                case RendererAPI::None:
                    break;

                case RendererAPI::OpenGL:
                    return MakeSharedPtr<glVertexArray>();

                case RendererAPI::Vulkan:
                break;
            }

            OASSERTM(false, "Unkown RendererAPI!");
            return nullptr;
        }

    }   // Shrimp

}   // Ocean