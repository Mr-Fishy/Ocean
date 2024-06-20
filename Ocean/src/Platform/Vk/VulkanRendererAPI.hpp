#pragma once

#include "Ocean/Renderer/RendererAPI.hpp"

namespace Ocean::Vk {

	class VulkanRendererAPI : public RendererAPI {
		virtual void Init() override final;

		virtual void SetViewport(ui32 x, ui32 y, ui32 width, ui32 height) override final;

		virtual void SetClearColor(const vec4& color) override final;

		virtual void Clear() override final;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, ui32 indexCount = 0) override final;
	};

}	// Ocean::Vk
