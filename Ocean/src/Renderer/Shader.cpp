#include "Shader.hpp"

#include "Ocean/Core/Types/SmartPtrs.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"

#include "Renderer/RendererAPI.hpp"
#include "Renderer/OpenGL/gl_Shader.hpp"

namespace Ocean {

    namespace Shrimp {

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

    }   // Shrimp

}   // Ocean