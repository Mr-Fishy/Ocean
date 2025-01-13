#include "gl_RendererAPI.hpp"

#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Assert.hpp"
#include "Ocean/Primitives/Macros.hpp"

#include "Ocean/Renderer/VertexArray.hpp"

// libs
#include <glad/gl.h>

namespace Ocean {

    namespace Shrimp {

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

        

        void glRendererAPI::Init() {
        #ifdef OC_DEBUG

            glDebugMessageCallback(glMessageCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

        #endif

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glEnable(GL_DEPTH_TEST);
        }

        void glRendererAPI::SetViewport(u32 x, u32 y, u32 w, u32 h) {
            glViewport(x, y, w, h);
        }

        void glRendererAPI::SetClearColor(const glm::vec4& color) {
            glClearColor(color.r, color.g, color.b, color.a);
        }

        void glRendererAPI::Clear() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void glRendererAPI::DrawIndexed(const Ref<VertexArray>& array, u32 indexCount) {
            u32 count = indexCount ? indexCount : array->GetIndexBuffer()->GetCount();

            glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

    }   // Shrimp

}   // Ocean
