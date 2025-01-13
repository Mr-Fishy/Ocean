#include "Framebuffer.hpp"

#include "Ocean/Core/Types/SmartPtrs.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"

#include "Renderer/RendererAPI.hpp"
#include "Renderer/OpenGL/gl_Framebuffer.hpp"

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
