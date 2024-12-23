#include "Texture.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"
#include "Ocean/Core/Types/SharedPtr.hpp"

#include "Renderer/RendererAPI.hpp"
#include "Renderer/OpenGL/gl_Texture.hpp"

namespace Ocean {

    namespace Shrimp {

        SharedPtr<Texture2D> Texture2D::Create(u32 width, u32 height) {
            switch (RendererAPI::GetAPI()) {
                case RendererAPI::None:
                    break;

                case RendererAPI::OpenGL:
                    return MakeSharedPtr<glTexture2D>(width, height);

                case RendererAPI::Vulkan:
                    break;
            }

            OASSERTM(false, "Unkown RendererAPI!");
            return nullptr;
        }
    
    }   // Shrimp

}   // Ocean
