#include "VertexBuffer.hpp"

#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Exceptions.hpp"

#include "Ocean/Renderer/RendererAPI.hpp"

#include "Ocean/Renderer/OpenGL/gl_VertexBuffer.hpp"
#include "Ocean/Renderer/Vulkan/vk_VertexBuffer.hpp"

namespace Ocean {

    namespace Splash {

        Ref<VertexBuffer> VertexBuffer::Create(u32 size) {
            switch (RendererAPI::GetAPI()) {
                case RendererAPI::None:
                    break;

                case RendererAPI::OpenGL:
                    return MakeRef<glVertexBuffer>(size);

                case RendererAPI::Vulkan:
                    return MakeRef<vkVertexBuffer>(size);
            }

            throw Exception(Error::YOU_FUCKED_UP, "Your not supposed to be here.");

            return nullptr;
        }

        Ref<VertexBuffer> VertexBuffer::Create(float* vertices, u32 size) {
            switch (RendererAPI::GetAPI()) {
                case RendererAPI::None:
                    break;

                case RendererAPI::OpenGL:
                    return MakeRef<glVertexBuffer>(vertices, size);

                case RendererAPI::Vulkan:
                    return MakeRef<vkVertexBuffer>(vertices, size);
            }

            throw Exception(Error::YOU_FUCKED_UP, "Your not supposed to be here.");

            return nullptr;
        }

    }   // Splash

}   // Ocean
