#include "VertexBuffer.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"

#include "Renderer/RendererAPI.hpp"
#include "Renderer/OpenGL/gl_VertexBuffer.hpp"

namespace Ocean {

    namespace Shrimp {

        Ref<VertexBuffer> VertexBuffer::Create(u32 size) {
            switch (RendererAPI::GetAPI()) {
                case RendererAPI::None:
                    break;

                case RendererAPI::OpenGL:
                    return MakeRef<glVertexBuffer>(size);

                case RendererAPI::Vulkan:
                    break;
            }

            OASSERTM(false, "Unkown RendereAPI!");
            return nullptr;
        }

        Ref<VertexBuffer> VertexBuffer::Create(float* vertices, u32 size) {
            switch (RendererAPI::GetAPI()) {
                case RendererAPI::None:
                    break;

                case RendererAPI::OpenGL:
                    return MakeRef<glVertexBuffer>(vertices, size);

                case RendererAPI::Vulkan:
                    break;
            }

            OASSERTM(false, "Unkown RendereAPI!");
            return nullptr;
        }

    }   // Shrimp

}   // Ocean