#include "GraphicsContext.hpp"

#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Assert.hpp"

#include "Ocean/Renderer/RendererAPI.hpp"

#include "Ocean/Renderer/OpenGL/gl_GraphicsContext.hpp"
#include "Ocean/Renderer/Vulkan/vk_GraphicsContext.hpp"

namespace Ocean {

    namespace Splash {
    
        Scope<GraphicsContext> GraphicsContext::Create(void* windowHandle) {
            switch (RendererAPI::GetAPI()) {
                case RendererAPI::None:
                    break;

                case Ocean::Splash::RendererAPI::API::OpenGL:
                    return MakeScope<glGraphicsContext>(static_cast<GLFWwindow*>(windowHandle));

                case Ocean::Splash::RendererAPI::API::Vulkan:
                    return MakeScope<vkGraphicsContext>(static_cast<GLFWwindow*>(windowHandle));
                }

            OASSERTM(false, "Unkown Rendering API!");
            return nullptr;
        }

    }   // Splash

}   // Ocean
