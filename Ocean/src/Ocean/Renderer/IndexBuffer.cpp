#include "IndexBuffer.hpp"

#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Exceptions.hpp"

#include "Ocean/Renderer/RendererAPI.hpp"

#include "Ocean/Renderer/OpenGL/gl_IndexBuffer.hpp"
#include "Ocean/Renderer/Vulkan/vk_IndexBuffer.hpp"

namespace Ocean {

    namespace Splash {
    
        Ref<IndexBuffer> IndexBuffer::Create(u32* indices, u32 count) {
            switch (RendererAPI::GetAPI()) {
                case RendererAPI::None:
                    break;

                case RendererAPI::OpenGL:
                    return MakeRef<glIndexBuffer>(indices, count);

                case RendererAPI::Vulkan: break;
                    // return MakeRef<vkIndexBuffer>(indices, count);
            }

            throw Exception(Error::YOU_FUCKED_UP, "Your not supposed to be here.");

            return nullptr;
        }

    }   // Splash

}   // Ocean
