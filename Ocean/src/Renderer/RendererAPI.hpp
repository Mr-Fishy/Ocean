#pragma once

// libs
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

namespace Ocean {

    namespace Render {
        
        // TODO: Transparency (4 component color)

        void DrawRect(glm::vec2 pos, glm::vec3 color);
        // void DrawRect(glm::vec2 pos, glm::vec4 color);

        void DrawRect(glm::vec3 pos, glm::vec3 color);
        // void DrawRect(glm::vec3 pos, glm::vec4 color);

        // TODO: Textures
        // void DrawRect(glm::vec2 pos, Texture texture);

        // TODO: Circles

        // TODO: Triangles

        // TODO: Polygons

    }   // Render

}   // Ocean
