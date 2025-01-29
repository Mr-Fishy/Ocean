#include "GraphicsContext.hpp"

#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Exceptions.hpp"

#include "Ocean/Renderer/RendererAPI.hpp"

#include "Ocean/Renderer/OpenGL/gl_GraphicsContext.hpp"
#include "Ocean/Renderer/Vulkan/vk_GraphicsContext.hpp"

namespace Ocean {

    namespace Splash {
    
        Scope<GraphicsContext> GraphicsContext::Create(void* windowHandle) {
            switch (RendererAPI::GetAPI()) {
                case RendererAPI::None:
                    break;

                case RendererAPI::API::OpenGL:
                    return MakeScope<glGraphicsContext>(static_cast<GLFWwindow*>(windowHandle));

                case RendererAPI::API::Vulkan:
                    return MakeScope<vkGraphicsContext>(static_cast<GLFWwindow*>(windowHandle));
            }

            throw Exception(Error::BAD_FUNCTION_CALL, "Your not supposed to be here.");

            return nullptr;
        }

    }   // Splash

}   // Ocean
