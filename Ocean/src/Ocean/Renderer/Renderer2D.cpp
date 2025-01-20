#include "Renderer2D.hpp"

#include "Camera/Camera.hpp"
#include "Ocean/Types/FloatingPoints.hpp"
#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Array.hpp"

#include "Ocean/Core/ResourceManager.hpp"

#include "Ocean/Renderer/IndexBuffer.hpp"
#include "Ocean/Renderer/RenderCommand.hpp"
#include "Ocean/Renderer/Shader.hpp"
#include "Ocean/Renderer/Texture.hpp"
#include "Ocean/Renderer/VertexArray.hpp"
#include "Ocean/Renderer/VertexBuffer.hpp"

#include "Ocean/Renderer/Camera/Camera.hpp"

// libs
#include <glm/ext/matrix_transform.hpp>

namespace Ocean {

    struct QuadVertex {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 texCoord;

        f32 texIndex;
        f32 tilingFactor;

        // Editor Specific
        i32 entityID;

    };  // QuadVertex

    struct RendererData {
        RendererData() : quadVertexArray(), quadVertexBuffer(), textureShader(), colorTexture(), textureSlots(), stats() { }
        RendererData(const RendererData&) = delete;
        ~RendererData() = default;

        RendererData& operator = (const RendererData&) = delete;

        static constexpr u32 maxQuads = 20000;
        static constexpr u32 maxVertices = maxQuads * 4;
        static constexpr u32 maxIndices = maxQuads * 6;
        static constexpr u32 maxTextureSlots = 32;

        Ref<Shrimp::VertexArray> quadVertexArray;
        Ref<Shrimp::VertexBuffer> quadVertexBuffer;

        Ref<Shrimp::Shader> textureShader;
        Ref<Shrimp::Texture2D> colorTexture;

        u32 quadIndexCount = 0;
        QuadVertex* quadVertexBufferBase = nullptr;
        QuadVertex* quadVertexBufferPtr = nullptr;

        FixedArray<Ref<Shrimp::Texture2D>, maxTextureSlots> textureSlots;
        u32 textureSlotIndex = 1; // 0 Is colorTexture

        glm::vec4 quadVertexPositions[4] = {
            { -0.5f, -0.5f, 0.0f, 1.0f },
            {  0.5f, -0.5f, 0.0f, 1.0f },
            {  0.5f,  0.5f, 0.0f, 1.0f },
            { -0.5f,  0.5f, 0.0f, 1.0f }
        };

        Renderer2D::Statistics stats;

    };  // RenderData



    static RendererData s_Data;

    void Renderer2D::Init() {
        s_Data.quadVertexArray = Shrimp::VertexArray::Create();

        s_Data.quadVertexBuffer = Shrimp::VertexBuffer::Create(s_Data.maxVertices * sizeof(QuadVertex));
        s_Data.quadVertexBuffer->SetLayout({
            { Shrimp::ShaderDataType::Float3, "a_Position"     },
            { Shrimp::ShaderDataType::Float4, "a_Color"        },
            { Shrimp::ShaderDataType::Float2, "a_TexCoord"     },
            { Shrimp::ShaderDataType::Float,  "a_TexIndex"     },
            { Shrimp::ShaderDataType::Float,  "a_TilingFactor" },
            { Shrimp::ShaderDataType::Int,    "a_EntityID"     },
        });
        s_Data.quadVertexArray->AddVertexBuffer(s_Data.quadVertexBuffer);

        s_Data.quadVertexBufferBase = new QuadVertex[s_Data.maxVertices];

        u32* quadIndices = new u32[s_Data.maxIndices];

        u32 offset = 0;
        for (u32 i = 0; i < s_Data.maxIndices; i += 6) {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }

        Ref<Shrimp::IndexBuffer> quadIB = Shrimp::IndexBuffer::Create(quadIndices, s_Data.maxIndices);
        s_Data.quadVertexArray->SetIndexBuffer(quadIB);
        delete[] quadIndices;

        s_Data.colorTexture = Shrimp::Texture2D::Create(1, 1);
        u32 textureData = 0xffffffff;
        s_Data.colorTexture->SetData(&textureData, sizeof(u32));

        i32 samplers[s_Data.maxTextureSlots] { };
        for (u32 i = 0; i < s_Data.maxTextureSlots; i++)
            samplers[i] = i;

        s_Data.textureShader = ResourceManager::LoadShader("../../assets/Shaders/Texture.glsl", "Texture");
        s_Data.textureShader->Bind();
        s_Data.textureShader->SetIntArray("u_Textures", samplers, s_Data.maxTextureSlots);

        // Set First Texture Slot To 0
        s_Data.textureSlots[0] = s_Data.colorTexture;
        s_Data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.quadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.quadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
        s_Data.quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
    }

    void Renderer2D::Shutdown() {
        delete[] s_Data.quadVertexBufferBase;
    }

    void Renderer2D::BeginScene(const Camera& camera) {
        s_Data.textureShader->Bind();
        s_Data.textureShader->SetMat4f("u_ViewProjection", camera.GetViewProjectionMatrix());

        StartBatch();
    }

    void Renderer2D::EndScene() {
        Flush();
    }

    void Renderer2D::Flush() {
        if (s_Data.quadIndexCount == 0)
            return;

        u32 dataSize = static_cast<u32>(reinterpret_cast<u8*>(s_Data.quadVertexBufferPtr) - reinterpret_cast<u8*>(s_Data.quadVertexBufferBase));
        s_Data.quadVertexBuffer->SetData(s_Data.quadVertexBufferBase, dataSize);

        for (u32 i = 0; i < s_Data.textureSlotIndex; i++)
            s_Data.textureSlots[i]->Bind(i);

        RenderCommand::DrawIndexed(s_Data.quadVertexArray, s_Data.quadIndexCount);
        s_Data.stats.drawCalls++;
    }

    

    void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color) {
        DrawQuad(
            { pos.x, pos.y, 0.0f },
            size,
            color
        );
    }

    void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color) {
        DrawQuad(
            glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f),
            { size.x, size.y, 1.0f }),
            color
        );
    }

    void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Shrimp::Texture2D>& texture, f32 tilingFactor, const glm::vec4& tintColor) {
        DrawQuad(
            { pos.x, pos.y, 0.0f },
            size,
            texture,
            tilingFactor,
            tintColor
        );
    }

    void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Shrimp::Texture2D>& texture, f32 tilingFactor, const glm::vec4& tintColor) {
        DrawQuad(
            glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }),
            texture,
            tilingFactor,
            tintColor
        );
    }



    void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4 color, i32 entityID) {
        if (s_Data.quadIndexCount >= RendererData::maxIndices)
            NextBatch();

        constexpr sizet quadVertexCount = 4;
        const glm::vec2 texCoords[4] = {
            { 0.0f, 0.0f },
            { 1.0f, 0.0f },
            { 1.0f, 1.0f },
            { 0.0f, 1.0f },
        };

        for (sizet i = 0; i < quadVertexCount; i++) {
            s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[i];
            s_Data.quadVertexBufferPtr->color = color;
            s_Data.quadVertexBufferPtr->texCoord = texCoords[i];
            s_Data.quadVertexBufferPtr->texIndex = 0.0f;		// White Texture is 0.0f
            s_Data.quadVertexBufferPtr->tilingFactor = 1.0f;	// 1.0f Tiling Factor
            s_Data.quadVertexBufferPtr->entityID = entityID;
            s_Data.quadVertexBufferPtr++;
        }

        s_Data.quadIndexCount += 6;

        s_Data.stats.quadCount++;
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Shrimp::Texture2D>& texture, f32 tilingFactor, const glm::vec4& tintColor, i32 entityID) {
        if (s_Data.quadIndexCount >= RendererData::maxIndices)
            NextBatch();

        constexpr sizet quadVertexCount = 4;
        const glm::vec2 texCoords[4] = {
            { 0.0f, 0.0f },
            { 1.0f, 0.0f },
            { 1.0f, 1.0f },
            { 0.0f, 1.0f },
        };

        f32 texIndex = 0.0f;
        for (u32 i = 1; i < s_Data.textureSlotIndex; i++) {
            if (*s_Data.textureSlots[i] == *texture) {
                texIndex = static_cast<f32>(i);

                break;
            }
        }

        if (texIndex == 0.0f) {
            if (s_Data.quadIndexCount >= RendererData::maxIndices)
                NextBatch();

            texIndex = static_cast<f32>(s_Data.textureSlotIndex);
            
            s_Data.textureSlots[s_Data.textureSlotIndex] = texture;
            s_Data.textureSlotIndex++;
        }

        for (sizet i = 0; i < quadVertexCount; i++) {
            s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[i];
            s_Data.quadVertexBufferPtr->color = tintColor;
            s_Data.quadVertexBufferPtr->texCoord = texCoords[i];
            s_Data.quadVertexBufferPtr->texIndex = texIndex;	        // White Texture is 0.0f
            s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;	// 1.0f Tiling Factor
            s_Data.quadVertexBufferPtr->entityID = entityID;
            s_Data.quadVertexBufferPtr++;
        }

        s_Data.quadIndexCount += 6;

        s_Data.stats.quadCount++;
    }



    void Renderer2D::ResetStats() {
        s_Data.stats = Renderer2D::Statistics();
    }

    Renderer2D::Statistics Renderer2D::GetStats() {
        return s_Data.stats;
    }



    void Renderer2D::StartBatch() {
        s_Data.quadIndexCount = 0;
        s_Data.quadVertexBufferPtr = s_Data.quadVertexBufferBase;

        s_Data.textureSlotIndex = 1;
    }

    void Renderer2D::NextBatch() {
        EndScene();

        s_Data.quadIndexCount = 0;
        s_Data.quadVertexBufferPtr = s_Data.quadVertexBufferBase;
        
        s_Data.textureSlotIndex = 1;
    } 

}   // Ocean
