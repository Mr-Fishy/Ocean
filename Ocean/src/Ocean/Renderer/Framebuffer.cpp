#include "Framebuffer.hpp"

#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Assert.hpp"

#include "Ocean/Renderer/RendererAPI.hpp"
#include "Ocean/Renderer/OpenGL/gl_Framebuffer.hpp"

namespace Ocean {

    namespace Shrimp {

        Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {
            switch (RendererAPI::GetAPI()) {
                case RendererAPI::None:
                    break;
                
                case RendererAPI::OpenGL:
                    return MakeRef<glFramebuffer>(spec);
                
                case RendererAPI::Vulkan:
                    break;
            }

            OASSERTM(false, "Unkown RendererAPI");
            return nullptr;
        }

    }   // Shrimp

}   // Ocean
