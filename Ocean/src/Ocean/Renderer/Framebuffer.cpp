#include "Framebuffer.hpp"

#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Exceptions.hpp"

#include "Ocean/Renderer/RendererAPI.hpp"

#include "Ocean/Renderer/OpenGL/gl_Framebuffer.hpp"
#include "Ocean/Renderer/Vulkan/vk_Framebuffer.hpp"

namespace Ocean {

    namespace Splash {

        Framebuffer::Framebuffer(const FramebufferSpecification& spec) :
            m_Specification(spec),
            m_ColorAttachmentSpecs(),
            m_DepthAttachmentSpec()
        {
            for (auto& textureSpec : this->m_Specification.attachments.specs) {
                if (!IsDepthFormat(textureSpec.textureFormat))
                    this->m_ColorAttachmentSpecs.EmplaceBack(textureSpec);
                else
                    this->m_DepthAttachmentSpec = textureSpec;
            }
        }

        Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {
            switch (RendererAPI::GetAPI()) {
                case RendererAPI::None:
                    break;

                case RendererAPI::OpenGL:
                    return MakeRef<glFramebuffer>(spec);

                case RendererAPI::Vulkan: break;
                    // return MakeRef<vkFramebuffer>(spec);
            }

            throw Exception(Error::YOU_FUCKED_UP, "Your not supposed to be here.");

            return nullptr;
        }

    }   // Splash

}   // Ocean
