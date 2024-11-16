#pragma once

#include "Ocean/Core/Primitives/List.hpp"
#include "Ocean/Core/Types/Integers.hpp"

// libs
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

namespace Ocean {

    namespace Vulkan {

        struct Vertex;

        class Buffer;

    }   // Vulkan

    /* 
     * Renderer2D Design
     * 
     */

    class Renderer2D {
    private:
        struct Statistics {
            u32 DrawCalls = 0;
            u32 VertCount = 0;
            u32 IndexCount = 0;

            u32 GetTotalVertexCount() const { return VertCount; }
            u32 GetTotalIndexCount() const { return IndexCount; }

        };  // Statistics

        struct Data {
            static constexpr u32 MaxTris = 40000;
            static constexpr u32 MaxVerts = MaxTris * 3;
            static constexpr u32 MaxIndices = MaxTris * 3;
            static constexpr u8 MaxTextureSlots = 32;

            DoubleLinkedList<Vulkan::Vertex> Vertices;
            Vulkan::Buffer* VertexBuffer;
            DoubleLinkedList<u32> Indices;
            Vulkan::Buffer* IndexBuffer;

        };  // Data

    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(/* const Camera& camera */);
        static void EndScene();
        static void Flush();

        // TODO: Transparency (4 component color)

        static void DrawRect(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
        static void DrawRect(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color);

        // static void DrawRect(const glm::vec2& pos, const glm::vec2& size, const Texture& texture);
        // static void DrawRect(const glm::vec3& pos, const glm::vec2& size, const Texture& texture);

        // TODO: Circles
        // static void DrawCirc(const glm::vec2& pos, f32 radius, const glm::vec4& color);
        // static void DrawCirc(const glm::vec3& pos, f32 radius, const glm::vec4& color);

        // static void DrawCirc(const glm::vec2& pos, f32 radius, const Texture& texture);
        // static void DrawCirc(const glm::vec3& pos, f32 radius, const Texture& texture);

        // TODO: Triangles
        // static void DrawTri(const glm::vec2& pos, f32 base, f32 height, const glm::vec4& color);
        // static void DrawTri(const glm::vec3& pos, f32 base, f32 height, const glm::vec4& color);

        // static void DrawTri(const glm::vec2& pos, f32 base, f32 height, const Texture& texture);
        // static void DrawTri(const glm::vec3& pos, f32 base, f32 height, const Texture& texture);

        // TODO: Polygons

        static void ResetStats();
        static void PrintStats();

    private:
        static void FlushAndReset();

        /* --- */

        static inline Data* s_Data = nullptr;

    };

}   // Ocean
