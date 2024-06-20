#pragma once

#include "Ocean/Renderer/VertexIndexBuffer.hpp"

namespace Ocean::GL {

	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(ui32 size);
		OpenGLVertexBuffer(f32* vertices, ui32 size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override final;
		virtual void Unbind() const override final;

		virtual const BufferLayout& GetLayout() const override final { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override final { m_Layout = layout; }

		virtual void SetData(const void* data, ui32 size) override final;

	private:
		ui32 m_RendererID;

		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(ui32* indices, ui32 count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override final;
		virtual void Unbind() const override final;

		virtual ui32 GetCount() const override final { return m_Count; }

	private:
		ui32 m_RendererID;

		ui32 m_Count;
	};

}	// Ocean::GL
