#pragma once

#include "Ocean/Core/Types/Integers.hpp"
#include "Ocean/Core/Types/SharedPtr.hpp"
#include "Ocean/Core/Types/UniquePtr.hpp"

#include "Renderer/RendererAPI.hpp"
#include "Renderer/VertexArray.hpp"

// libs
#include <glm/glm.hpp>

namespace Ocean {

    namespace Shrimp {    

        class RenderCommand {
        public:
            inline static void Init() {
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

            inline static void DrawIndexed(const SharedPtr<VertexArray>& array, u32 count = 0) {
                s_RendererAPI->DrawIndexed(array, count);
            }

        private:
            inline static UniquePtr<RendererAPI> s_RendererAPI = RendererAPI::Create();

        };  // RenderCommand

    }   // Shrimp

}   // Ocean
