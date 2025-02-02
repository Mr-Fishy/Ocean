#include "RendererAPI.hpp"

#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Exceptions.hpp"

#include "Ocean/Platform/WindowContext.hpp"

#include "Ocean/Renderer/OpenGL/gl_RendererAPI.hpp"
#include "Ocean/Renderer/Vulkan/vk_RendererAPI.hpp"

namespace Ocean {

    namespace Splash {

        RendererAPI::RendererAPI() {
            WindowContext::Init();
        }

        RendererAPI::~RendererAPI() {
            WindowContext::Shutdown();
        }

        Scope<RendererAPI> RendererAPI::Create() {
            switch (s_API) {
                case None:
                    break;

                case OpenGL:
                    return MakeScope<glRendererAPI>();

                case Vulkan:
                    return MakeScope<vkRendererAPI>();
            }

            throw Exception(Error::BAD_FUNCTION_CALL, "Your not supposed to be here.");
            return nullptr;
        }

    }   // Splash

}   // Ocean
