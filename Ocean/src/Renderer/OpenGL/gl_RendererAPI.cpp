#include "gl_RendererAPI.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"
#include "Ocean/Core/Primitives/Macros.hpp"

#include "Ocean/Core/Types/SharedPtr.hpp"
#include "Renderer/VertexArray.hpp"
#include "Renderer/IndexBuffer.hpp"

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
                    oprint(CONSOLE_TEXT_RED("%s"), message);
                    return;

                case GL_DEBUG_SEVERITY_MEDIUM:
                    oprint(CONSOLE_TEXT_RED("%s"), message);
                    return;

                case GL_DEBUG_SEVERITY_LOW:
                    oprint(CONSOLE_TEXT_YELLOW("%s"), message);
                    return;

                case GL_DEBUG_SEVERITY_NOTIFICATION:
                    oprint(CONSOLE_TEXT_GREEN("%s"), message);
                    return;
            }

            OASSERTM(false, "Unkown OpenGL Message Severity Level!");
        }


    
        glRendererAPI::glRendererAPI() {

        }

        glRendererAPI::~glRendererAPI() {

        }

        void glRendererAPI::Init() {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        #ifdef OC_DEBUG

            glDebugMessageCallback(glMessageCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

        #endif

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

        void glRendererAPI::DrawIndexed(const SharedPtr<VertexArray>& array, u32 indexCount) {
            u32 count = indexCount ? indexCount : array->GetIndexBuffer()->GetCount();

            glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

    }   // Shrimp

}   // Ocean
