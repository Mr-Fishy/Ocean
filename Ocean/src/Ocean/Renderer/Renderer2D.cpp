#include "ocpch.hpp"

#include "Ocean/Renderer/Renderer2D.hpp"

#include "Ocean/Renderer/VertexArray.hpp"
#include "Ocean/Renderer/Shader.hpp"
#include "Ocean/Renderer/RenderCommand.hpp"

// libs
#include <glm/gtc/matrix_transform.hpp>

namespace Ocean {

	struct QuadVertex {
		vec3 Position;
		vec4 Color;
		vec2 TexCoord;

		f32 TexIndex;
		f32 TilingFactor;
	};

	struct Renderer2DData {
		static const ui32 MaxQuads = 20000;
		static const ui32 MaxVertices = MaxQuads * 4;
		static const ui32 MaxIndices = MaxQuads * 6;
		static const ui8 MaxTextureSlots = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		ui32 QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots{};
		ui32 TextureSlotIndex = 1; // 0 = White Texture

		vec4 QuadVertexPositions[4] = {
			{ -0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f,  0.5f, 0.0f, 1.0f },
			{ -0.5f,  0.5f, 0.0f, 1.0f }
		};

		Renderer2D::RendererStats Stats;
	};

	static Renderer2DData s_Data;

	void Renderer2D::ResetStats() {
		memset(&s_Data.Stats, 0, sizeof(RendererStats));
	}

	Renderer2D::RendererStats Renderer2D::GetStats() {
		return s_Data.Stats;
	}

	void Renderer2D::Init() {
		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"     },
			{ ShaderDataType::Float4, "a_Color"        },
			{ ShaderDataType::Float2, "a_TexCoord"     },
			{ ShaderDataType::Float,  "a_TexIndex"     },
			{ ShaderDataType::Float,  "a_TilingFactor" },
		});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
		ui32* quadIndices = new ui32[s_Data.MaxIndices];

		ui32 offset = 0;
		for (ui32 i = 0; i < s_Data.MaxIndices; i += 6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		ui32 whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(ui32));

		i32 samplers[s_Data.MaxTextureSlots]{};
		for (ui32 i = 0; i < s_Data.MaxTextureSlots; i++) {
			samplers[i] = i;
		}

		s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		// Set First Texture Slot To 0
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;
		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown() {
		delete[] s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetProjectionViewMatrix());

		StartBatch();
	}

	void Renderer2D::EndScene() {
		Flush();
	}

	void Renderer2D::Flush() {
		if (s_Data.QuadIndexCount == 0)
			return; // Nothing To Draw

		ui32 dataSize = (ui32)((ui8*)s_Data.QuadVertexBufferPtr - (ui8*)s_Data.QuadVertexBufferBase);
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		// Bind Textures
		for (ui32 i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		s_Data.Stats.DrawCalls++;
	}

	void Renderer2D::StartBatch() {
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::NextBatch() {
		EndScene();

		StartBatch();
	}

	/* --- Color Quads --- */

	void Renderer2D::DrawQuad(const vec2& position, const vec2& size, const vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const vec3& position, const vec2& size, const vec4& color) {
		mat4 transform = glm::translate(mat4(1.0f), position) * glm::scale(mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	/* --- Texture Quads --- */

	void Renderer2D::DrawQuad(const vec2& position, const vec2& size, Ref<Texture2D>& texture, float tilingFactor, const vec4& tintColor) {
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const vec3& position, const vec2& size, Ref<Texture2D>& texture, float tilingFactor, const vec4& tintColor) {
		mat4 transform = glm::translate(mat4(1.0f), position) * glm::scale(mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	/* --- Draw Quad Backend --- */

	void Renderer2D::DrawQuad(const mat4& transform, const vec4& color) {
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		constexpr ui64 quadVertexCount = 4;
		const vec2 textureCoords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		for (ui64 i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = 0.0f; // White Texture
			s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;

			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const mat4& transform, Ref<Texture2D>& texture, float tilingFactor, const vec4& tintColor) {
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		const ui64 quadVertexCount = 4;
		const vec2 textureCoords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		f32 textureIndex = 0.0f;
		for (ui32 i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i] == *texture) {
				textureIndex = static_cast<f32>(i);
				break;
			}
		}

		if (textureIndex == 0.0f) {
			if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
				NextBatch();

			textureIndex = static_cast<f32>(s_Data.TextureSlotIndex);
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (ui64 i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex; // White Texture
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}

}	// Ocean
