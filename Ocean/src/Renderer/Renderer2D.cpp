#include "Renderer2D.hpp"

#include "Ocean/Core/Primitives/Memory.hpp"

namespace Ocean {

    void Renderer2D::Init() {
        s_Data = oallocat(Data, 1, MemoryService::Instance().SystemAllocator());

        
    }

    void Renderer2D::Shutdown() {

    }



    void Renderer2D::BeginScene() {

    }

    void Renderer2D::EndScene() {

    }

    void Renderer2D::Flush() {

    }



    void Renderer2D::DrawRect(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color) {

    }

    void Renderer2D::DrawRect(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color) {

    }

}   // Ocean