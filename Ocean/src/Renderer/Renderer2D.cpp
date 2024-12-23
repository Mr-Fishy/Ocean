#include "Renderer2D.hpp"

#include "Ocean/Core/Types/FloatingPoints.hpp"
#include "Ocean/Core/Types/Integers.hpp"
#include "Ocean/Core/Types/SharedPtr.hpp"

#include "Ocean/Core/Primitives/Memory.hpp"
#include "Ocean/Core/Primitives/Array.hpp"

#include "Ocean/Core/ResourceManager.hpp"

#include "Ocean/Scene/Camera.hpp"

#include "Renderer/IndexBuffer.hpp"
#include "Renderer/RenderCommand.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Texture.hpp"
#include "Renderer/VertexArray.hpp"
#include "Renderer/VertexBuffer.hpp"

// std
#include <cstring>
#include <glm/ext/matrix_transform.hpp>

namespace Ocean {

    namespace Shrimp {

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
            static constexpr u32 maxQuads = 20000;
            static constexpr u32 maxVertices = maxQuads * 4;
            static constexpr u32 maxIndices = maxQuads * 6;
            static constexpr u32 maxTextureSlots = 32;

            SharedPtr<VertexArray> quadVertexArray;
            SharedPtr<VertexBuffer> quadVertexBuffer;

            SharedPtr<Shader> textureShader;
            SharedPtr<Texture2D> colorTexture;

            u32 quadIndexCount = 0;
            QuadVertex* quadVertexBufferBase = nullptr;
            QuadVertex* quadVertexBufferPtr = nullptr;

            FixedArray<SharedPtr<Texture2D>, maxTextureSlots> textureSlots;
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
            s_Data.quadVertexArray = VertexArray::Create();

            s_Data.quadVertexBuffer = VertexBuffer::Create(s_Data.maxVertices * sizeof(QuadVertex));
            s_Data.quadVertexBuffer->SetLayout({
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float4, "a_Color" },
                { ShaderDataType::Float2, "a_TexCoord" },
                { ShaderDataType::Float, "a_TexIndex" },
                { ShaderDataType::Float, "a_TilingFactor" }
            });
            s_Data.quadVertexArray->AddVertexBuffer(s_Data.quadVertexBuffer);

            s_Data.quadVertexBufferBase = oallocat(QuadVertex, s_Data.maxVertices, Ocean::MemoryService::Instance().SystemAllocator());

            u32* quadIndices = oallocat(u32, s_Data.maxIndices, Ocean::MemoryService::Instance().SystemAllocator());

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

            SharedPtr<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.maxIndices);
            s_Data.quadVertexArray->SetIndexBuffer(quadIB);
            ofree(quadIndices, Ocean::MemoryService::Instance().SystemAllocator());

            s_Data.colorTexture = Texture2D::Create(1, 1);
            u32 textureData = 0xffffffff;
            s_Data.colorTexture->SetData(&textureData, sizeof(u32));

            i32 samplers[s_Data.maxTextureSlots] { };
            for (u32 i = 0; i < s_Data.maxTextureSlots; i++) {
                samplers[i] = i;
            }

            s_Data.textureShader = ResourceManager::LoadShader("assets/Shaders/Texture.glsl", "Texture");
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
            ofree(s_Data.quadVertexBufferBase, Ocean::MemoryService::Instance().SystemAllocator());
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

            u32 dataSize = static_cast<u32>(s_Data.quadVertexBufferPtr - s_Data.quadVertexBufferBase);
            s_Data.quadVertexBuffer->SetData(s_Data.quadVertexBufferBase, dataSize);

            for (u32 i = 0; i < s_Data.textureSlotIndex; i++)
                s_Data.textureSlots[i]->Bind(i);

            RenderCommand::DrawIndexed(s_Data.quadVertexArray, s_Data.quadIndexCount);
            s_Data.stats.drawCalls++;
        }

        

        void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color) {
            DrawQuad({ pos.x, pos.y, 0.0f }, size, color);
        }

        void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color) {
            DrawQuad(
                glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }),
                color
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

        void Renderer2D::DrawQuad(const glm::mat4& transform, const SharedPtr<Texture2D>& texture, f32 tilingFactor, const glm::vec4& tintColor, i32 entityID) {

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

    }   // Shrimp

}   // Ocean
