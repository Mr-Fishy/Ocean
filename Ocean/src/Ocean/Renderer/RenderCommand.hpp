#pragma once

#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Renderer/RendererAPI.hpp"
#include "Ocean/Renderer/VertexArray.hpp"

// libs
#include <glm/glm.hpp>

namespace Ocean {

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

        inline static void DrawIndexed(const Ref<Shrimp::VertexArray>& array, u32 count = 0) {
            s_RendererAPI->DrawIndexed(array, count);
        }

    private:
        inline static Scope<Shrimp::RendererAPI> s_RendererAPI = Shrimp::RendererAPI::Create();

    };  // RenderCommand

}   // Ocean
