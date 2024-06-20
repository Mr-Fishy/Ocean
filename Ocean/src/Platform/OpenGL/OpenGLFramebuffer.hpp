#pragma once

#include "Ocean/Renderer/Framebuffer.hpp"

namespace Ocean::GL {

	class OpenGLFramebuffer : public Framebuffer {
	public:
		OpenGLFramebuffer(const FramebufferSpec& spec);
		virtual ~OpenGLFramebuffer();

		virtual void Bind() override final;
		virtual void Unbind() override final;

		virtual void Resize(ui32 width, ui32 height) override final;
		virtual int ReadPixel(ui32 attachmentIndex, i32 x, i32 y) override final;

		virtual ui32 GetColorAttachmentID(ui32 index = 0) const override final;
		virtual void ClearAttachment(ui32 attachmentIndex, i32 value) override final;

		void Invalidate();

		virtual const FramebufferSpec& GetSpec() const override final { return m_Specification; }

	private:
		ui32 m_RendererID = 0;

		FramebufferSpec m_Specification;

		std::vector<FramebufferTextureSpec> m_ColorAttachmentSpecs;
		FramebufferTextureSpec m_DepthAttachmentSpec = FramebufferFormat::None;

		std::vector<ui32> m_ColorAttachments;
		ui32 m_DepthAttachment = 0;
	};

}	// Ocean::GL
