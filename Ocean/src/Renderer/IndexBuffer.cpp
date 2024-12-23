#include "IndexBuffer.hpp"

#include "Ocean/Core/Types/SharedPtr.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"

#include "Renderer/RendererAPI.hpp"
#include "Renderer/OpenGL/gl_IndexBuffer.hpp"

namespace Ocean {

    namespace Shrimp {
    
        SharedPtr<IndexBuffer> IndexBuffer::Create(u32* indices, u32 count) {
            switch (RendererAPI::GetAPI()) {
                case RendererAPI::None:
                    break;

                case RendererAPI::OpenGL:
                    return MakeSharedPtr<glIndexBuffer>(indices, count);

                case RendererAPI::Vulkan:
                    break;
            }

            OASSERTM(false, "Unkown RendererAPI!");
            return nullptr;
        }

    }   // Shrimp

}   // Ocean
