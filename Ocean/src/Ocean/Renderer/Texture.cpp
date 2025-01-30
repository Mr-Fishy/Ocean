#include "Texture.hpp"

#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Exceptions.hpp"

#include "Ocean/Renderer/RendererAPI.hpp"

#include "Ocean/Renderer/OpenGL/gl_Texture.hpp"
#include "Ocean/Renderer/Vulkan/vk_Texture.hpp"

namespace Ocean {

    namespace Splash {

        Ref<Texture2D> Texture2D::Create(u32 width, u32 height) {
            switch (RendererAPI::GetAPI()) {
                case RendererAPI::None:
                    break;

                case RendererAPI::OpenGL:
                    return MakeRef<glTexture2D>(width, height);

                case RendererAPI::Vulkan:
                    return MakeRef<vkTexture2D>(width, height);
            }

            throw Exception(Error::BAD_FUNCTION_CALL, "Your not supposed to be here.");

            return nullptr;
        }
    
    }   // Splash

}   // Ocean
