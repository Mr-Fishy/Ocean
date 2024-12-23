#include "VertexBuffer.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"

#include "Renderer/RendererAPI.hpp"
#include "Renderer/OpenGL/gl_VertexBuffer.hpp"

namespace Ocean {

    namespace Shrimp {

        SharedPtr<VertexBuffer> VertexBuffer::Create(u32 size) {
            switch (RendererAPI::GetAPI()) {
                case RendererAPI::None:
                    break;

                case RendererAPI::OpenGL:
                    return MakeSharedPtr<glVertexBuffer>(size);

                case RendererAPI::Vulkan:
                    break;
            }

            OASSERTM(false, "Unkown RendereAPI!");
            return nullptr;
        }

        SharedPtr<VertexBuffer> VertexBuffer::Create(float* vertices, u32 size) {
            switch (RendererAPI::GetAPI()) {
                case RendererAPI::None:
                    break;

                case RendererAPI::OpenGL:
                    return MakeSharedPtr<glVertexBuffer>(vertices, size);

                case RendererAPI::Vulkan:
                    break;
            }

            OASSERTM(false, "Unkown RendereAPI!");
            return nullptr;
        }

    }   // Shrimp

}   // Ocean