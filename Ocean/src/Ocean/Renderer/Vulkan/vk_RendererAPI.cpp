#include "vk_RendererAPI.hpp"

#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Assert.hpp"
#include "Ocean/Primitives/Macros.hpp"

#include "Ocean/Renderer/VertexArray.hpp"

// libs
#include <glad/gl.h>

namespace Ocean {

    namespace Splash {

        static void glMessageCallback(
            OC_UNUSED u32 source, OC_UNUSED u32 type, OC_UNUSED u32 id,
            u32 severity,
            OC_UNUSED i32 length,
            const char* message,
            OC_UNUSED const void* userParam
        ) {
            switch (severity) {
                case GL_DEBUG_SEVERITY_HIGH:
                    oprint(CONSOLE_TEXT_RED("\n%s\n"), message);
                    OASSERT(false);
                    return;

                case GL_DEBUG_SEVERITY_MEDIUM:
                    oprint(CONSOLE_TEXT_RED("\n%s\n"), message);
                    return;

                case GL_DEBUG_SEVERITY_LOW:
                    oprint(CONSOLE_TEXT_YELLOW("\n%s\n"), message);
                    return;

                case GL_DEBUG_SEVERITY_NOTIFICATION:
                    oprint(CONSOLE_TEXT_GREEN("\n%s\n"), message);
                    return;
            }

            OASSERTM(false, "Unkown OpenGL Message Severity Level!");
        }

        

        void vkRendererAPI::Init() {

        }

        void vkRendererAPI::SetViewport(u32 x, u32 y, u32 w, u32 h) {

        }

        void vkRendererAPI::SetClearColor(const glm::vec4& color) {

        }

        void vkRendererAPI::Clear() {

        }

        void vkRendererAPI::DrawIndexed(const Ref<VertexArray>& array, u32 indexCount) {
            u32 count = indexCount ? indexCount : array->GetIndexBuffer()->GetCount();

            
        }

    }   // Splash

}   // Ocean
