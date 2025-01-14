#pragma once

#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/SmartPtrs.hpp"

// libs
#include <glm/glm.hpp>

namespace Ocean {

    namespace Shrimp {

    class VertexArray;
    
    class RendererAPI {
    public:
        enum API {
            None = 0,

            OpenGL = 1,
            Vulkan = 2,

        };  // API

        /* --- */

        virtual ~RendererAPI() = default;

        virtual void Init() = 0;

        virtual void SetViewport(u32 x, u32 y, u32 w, u32 h) = 0;

        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed(const Ref<VertexArray>& array, u32 indexCount = 0) = 0;

        static API GetAPI() { return s_API; }

        static Scope<RendererAPI> Create();

    private:
        static inline API s_API = API::OpenGL;

    };  // RendererAPI

    }   // Shrimp

}   // Ocean
