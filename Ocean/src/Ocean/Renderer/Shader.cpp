#include "Shader.hpp"

#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Assert.hpp"

#include "Ocean/Renderer/RendererAPI.hpp"

#include "Ocean/Renderer/OpenGL/gl_Shader.hpp"

namespace Ocean {

    namespace Splash {

        Ref<Shader> Shader::Create(cstring vertexSource, cstring fragmentSource, cstring geometrySource) {
            switch (RendererAPI::GetAPI()) {
                case RendererAPI::None:
                    break;

                case RendererAPI::OpenGL:
                    return MakeRef<glShader>(vertexSource, fragmentSource, geometrySource);

                case RendererAPI::Vulkan:
                    break;
            }

            OASSERTM(false, "Unkown RendererAPI!");
            return nullptr;
        }

    }   // Splash

}   // Ocean