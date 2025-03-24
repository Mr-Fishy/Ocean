#include "VertexArray.hpp"

#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Exceptions.hpp"

#include "Ocean/Renderer/RendererAPI.hpp"

#include "Ocean/Renderer/OpenGL/gl_VertexArray.hpp"
#include "Ocean/Renderer/Vulkan/vk_VertexArray.hpp"

namespace Ocean {

    namespace Splash {

        Ref<VertexArray> VertexArray::Create() {
            switch (RendererAPI::GetAPI()) {
                case RendererAPI::None:
                    break;

                case RendererAPI::OpenGL:
                    return MakeRef<glVertexArray>();

                case RendererAPI::Vulkan: break;
                    // return MakeRef<vkVertexArray>();
            }

            throw Exception(Error::YOU_FUCKED_UP, "Your not supposed to be here.");

            return nullptr;
        }

    }   // Splash

}   // Ocean
