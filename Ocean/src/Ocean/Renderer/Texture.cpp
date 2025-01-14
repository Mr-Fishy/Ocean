#include "Texture.hpp"

#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Assert.hpp"

#include "Ocean/Renderer/RendererAPI.hpp"
#include "Ocean/Renderer/OpenGL/gl_Texture.hpp"

namespace Ocean {

    namespace Shrimp {

        Ref<Texture2D> Texture2D::Create(u32 width, u32 height) {
            switch (RendererAPI::GetAPI()) {
                case RendererAPI::None:
                    break;

                case RendererAPI::OpenGL:
                    return MakeRef<glTexture2D>(width, height);

                case RendererAPI::Vulkan:
                    break;
            }

            OASSERTM(false, "Unkown RendererAPI!");
            return nullptr;
        }
    
    }   // Shrimp

}   // Ocean
