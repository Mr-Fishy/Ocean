#pragma once

#include "Ocean/Renderer/VertexArray.hpp"

namespace Ocean::GL {

	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override final;
		virtual void Unbind() const override final;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override final;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override final;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override final { return m_VertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override final { return m_IndexBuffer; }

	private:
		ui32 m_RendererID;

		ui32 m_VertexBufferIndex = 0;

		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};

}	// Ocean::GL
