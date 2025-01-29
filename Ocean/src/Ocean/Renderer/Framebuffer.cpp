#include "Framebuffer.hpp"

#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Exceptions.hpp"

#include "Ocean/Renderer/RendererAPI.hpp"

#include "Ocean/Renderer/OpenGL/gl_Framebuffer.hpp"
#include "Ocean/Renderer/Vulkan/vk_Framebuffer.hpp"

namespace Ocean {

    namespace Splash {

        Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {
            switch (RendererAPI::GetAPI()) {
                case RendererAPI::None:
                    break;

                case RendererAPI::OpenGL:
                    return MakeRef<glFramebuffer>(spec);

                case RendererAPI::Vulkan:
                    return MakeRef<vkFramebuffer>(spec);
            }

            throw Exception(Error::BAD_FUNCTION_CALL, "Your not supposed to be here.");

            return nullptr;
        }

    }   // Splash

}   // Ocean
