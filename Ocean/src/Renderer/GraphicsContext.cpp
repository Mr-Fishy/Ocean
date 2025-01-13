#include "GraphicsContext.hpp"

#include "Ocean/Core/Types/SmartPtrs.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"

#include "Renderer/RendererAPI.hpp"

#include "Renderer/OpenGL/gl_GraphicsContext.hpp"
#include "Renderer/Vulkan/vk_GraphicsContext.hpp"

namespace Ocean {

    namespace Shrimp {
    
        Scope<GraphicsContext> GraphicsContext::Create(void* window) {
            switch (RendererAPI::GetAPI()) {
                case RendererAPI::None:
                    break;

                case Ocean::Shrimp::RendererAPI::API::OpenGL:
                    return MakeScope<glGraphicsContext>(static_cast<GLFWwindow*>(window));

                case Ocean::Shrimp::RendererAPI::API::Vulkan:
                    return MakeScope<vkGraphicsContext>(static_cast<GLFWwindow*>(window));
                }

            OASSERTM(false, "Unkown Rendering API!");
            return nullptr;
        }

    }   // Shrimp

}   // Ocean
