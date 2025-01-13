#pragma once

#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Renderer/RendererAPI.hpp"
#include "Ocean/Renderer/VertexArray.hpp"

// libs
#include <glm/glm.hpp>

namespace Ocean {

    namespace Shrimp {    

        class RenderCommand {
        public:
            inline static void Init() {
                if (!s_RendererAPI.get())
                    s_RendererAPI = RendererAPI::Create();

                s_RendererAPI->Init();
            }

            inline static void SetViewport(u32 x, u32 y, u32 w, u32 h) {
                s_RendererAPI->SetViewport(x, y, w, h);
            }
            inline static void SetClearColor(const glm::vec4& color) {
                s_RendererAPI->SetClearColor(color);
            }

            inline static void Clear() {
                s_RendererAPI->Clear();
            }

            inline static void DrawIndexed(const Ref<VertexArray>& array, u32 count = 0) {
                s_RendererAPI->DrawIndexed(array, count);
            }

        private:
            inline static Scope<RendererAPI> s_RendererAPI;

        };  // RenderCommand

    }   // Shrimp

}   // Ocean
