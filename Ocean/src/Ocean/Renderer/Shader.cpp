#include "Shader.hpp"

#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Exceptions.hpp"

#include "Ocean/Renderer/RendererAPI.hpp"

#include "Ocean/Renderer/OpenGL/gl_Shader.hpp"
#include "Ocean/Renderer/Vulkan/vk_Shader.hpp"

// libs
#include <glslang/SPIRV/GlslangToSpv.h>

namespace Ocean {

    namespace Splash {

        DynamicArray<u32> Shader::Compiler::CompileToSpirv(const DynamicArray<u8>& source) {
            
            
        
        }

        Ref<Shader> Shader::Create(const DynamicArray<i8>& vertexSource, const DynamicArray<i8>& fragmentSource, const DynamicArray<i8>& geometrySource) {
            switch (RendererAPI::GetAPI()) {
                case RendererAPI::None:
                    break;

                case RendererAPI::OpenGL:
                    return MakeRef<glShader>(vertexSource, fragmentSource, geometrySource);

                case RendererAPI::Vulkan:
                    return MakeRef<vkShader>(vertexSource, fragmentSource, geometrySource);
            }

            throw Exception(Error::BAD_FUNCTION_CALL, "Your not supposed to be here.");

            return nullptr;
        }

    }   // Splash

}   // Ocean
