#pragma once

#include "Ocean/Renderer/VertexIndexBuffer.hpp"

namespace Ocean::Vk {

	class VulkanVertexBuffer : public VertexBuffer {
	public:
		VulkanVertexBuffer(ui32 size);
		VulkanVertexBuffer(f32* vertices, ui32 size);
		virtual ~VulkanVertexBuffer();

		virtual void Bind() const override final;
		virtual void Unbind() const override final;

		virtual const BufferLayout& GetLayout() const override final { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override final { m_Layout = layout; }

		virtual void SetData(const void* data, ui32 size) override final;

	private:
		BufferLayout m_Layout;
	};

	class VulkanIndexBuffer : public IndexBuffer {
	public:
		VulkanIndexBuffer(ui32* indices, ui32 count);
		virtual ~VulkanIndexBuffer();

		virtual void Bind() const override final;
		virtual void Unbind() const override final;

		virtual ui32 GetCount() const override final { return m_Count; }

	private:
		ui32 m_Count;
	};

}	// Ocean::Vk
