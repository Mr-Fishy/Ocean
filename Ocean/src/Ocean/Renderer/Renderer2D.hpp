#pragma once

#include "Ocean/Renderer/Texture.hpp"

#include "Ocean/Renderer/Camera.hpp"

namespace Ocean {

	class Renderer2D {
	public:
		struct RendererStats {
			ui32 DrawCalls = 0;
			ui32 QuadCount = 0;

			ui32 GetTotalVertexCount() const { return QuadCount * 4; }
			ui32 GetTotalIndexCount() const { return QuadCount * 6; }
		};

		static void ResetStats();
		static RendererStats GetStats();

		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		// static void BeginScene(const PerspectiveCamera& camera);
		static void EndScene();

		static void Flush();

	private:
		static void StartBatch();
		static void NextBatch();

	public:
		static void DrawQuad(const vec2& position, const vec2& size, const vec4& color);
		static void DrawQuad(const vec3& position, const vec2& size, const vec4& color);

		static void DrawQuad(const vec2& position, const vec2& size, Ref<Texture2D>& texture, float tilingFactor = 1.0f, const vec4& tintColor = vec4(1.0f));
		static void DrawQuad(const vec3& position, const vec2& size, Ref<Texture2D>& texture, float tilingFactor = 1.0f, const vec4& tintColor = vec4(1.0f));

	private:
		static void DrawQuad(const mat4& transform, const vec4& color);
		static void DrawQuad(const mat4& transform, Ref<Texture2D>& texture, float tilingFactor = 1.0f, const vec4& tintColor = vec4(1.0f));
	};

}	// Ocean
