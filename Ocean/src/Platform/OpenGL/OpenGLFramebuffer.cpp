#include "ocpch.hpp"

#include "Platform/OpenGL/OpenGLFramebuffer.hpp"

// libs
#include <glad/gl.h>

namespace Ocean::GL {

	namespace Internal {

		static GLenum TextureTarget(b8 multisampled) {
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(b8 multisampled, ui32* outID, ui32 count) {
			glCreateTextures(TextureTarget(multisampled), count, outID);
		}

		static void BindTexture(b8 multisampled, ui32 id) {
			glBindTexture(TextureTarget(multisampled), id);
		}

		static void AttachColorTexture(ui32 id, i32 samples, GLenum internalFormat, GLenum format, ui32 width, ui32 height, i32 index) {
			b8 multisampled = samples > 1;

			if (multisampled) {
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else {
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
		}

		static void AttachDepthTexture(ui32 id, i32 samples, GLenum format, GLenum attachmentType, ui32 width, ui32 height) {
			b8 multisampled = samples > 1;

			if (multisampled) {
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else {
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
		}

		static b8 IsDepthFormat(FramebufferFormat format) {
			switch (format) {
				case Ocean::FramebufferFormat::DEPTH24STENCIL8:
					return true;
			}

			return false;
		}

		static GLenum OceanFBTextureFormatToGL(FramebufferFormat format) {
			switch (format) {
				case Ocean::FramebufferFormat::RGBA8:
					return GL_RGBA8;

				case Ocean::FramebufferFormat::RED_INTEGER:
					return GL_RED_INTEGER;
			}

			return 0;
		}

	}	// Internal

	static const ui32 s_MaxFramebufferSize = 8192;

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpec& spec) : m_Specification(spec) {
		for (auto& spec : m_Specification.AttachmentSpecs.Attachments) {
			if (!Internal::IsDepthFormat(spec.TextureFormat))
				m_ColorAttachmentSpecs.emplace_back(spec);
			else
				m_DepthAttachmentSpec = spec;
		}

		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer() {
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFramebuffer::Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLFramebuffer::Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(ui32 width, ui32 height) {
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize) {
			return;
		}

		m_Specification.Width = width;
		m_Specification.Height = height;

		Invalidate();
	}

	int OpenGLFramebuffer::ReadPixel(ui32 attachmentIndex, i32 x, i32 y) {
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);

		i32 pixelData = -1;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

		return pixelData;
	}

	ui32 OpenGLFramebuffer::GetColorAttachmentID(ui32 index) const {
		if (index >= m_ColorAttachments.size())
			throw std::runtime_error("Attempt to access invalid color attachment ID!");

		return m_ColorAttachments[index];
	}

	void OpenGLFramebuffer::ClearAttachment(ui32 attachmentIndex, i32 value) {
		auto& spec = m_ColorAttachmentSpecs[attachmentIndex];

		glClearTexImage(
			m_ColorAttachments[attachmentIndex], 0,
			Internal::OceanFBTextureFormatToGL(spec.TextureFormat),
			GL_INT, &value
		);
	}

	void OpenGLFramebuffer::Invalidate() {
		if (m_RendererID) {
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		b8 multisample = m_Specification.Samples > 1;

		if (m_ColorAttachmentSpecs.size()) {
			m_ColorAttachments.resize(m_ColorAttachmentSpecs.size());
			Internal::CreateTextures(multisample, m_ColorAttachments.data(), static_cast<ui32>(m_ColorAttachments.size()));

			for (int i = 0; i < m_ColorAttachments.size(); i++) {
				Internal::BindTexture(multisample, m_ColorAttachments[i]);

				switch (m_ColorAttachmentSpecs[i].TextureFormat) {
					case FramebufferFormat::RGBA8:
						Internal::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
						break;

					case FramebufferFormat::RED_INTEGER:
						Internal::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
						break;
				}
			}
		}

		if (m_DepthAttachmentSpec.TextureFormat != FramebufferFormat::None) {
			Internal::CreateTextures(multisample, &m_DepthAttachment, 1);
			Internal::BindTexture(multisample, m_DepthAttachment);

			switch (m_DepthAttachmentSpec.TextureFormat) {
				case FramebufferFormat::DEPTH24STENCIL8:
					Internal::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
					break;
			}
		}

		if (m_ColorAttachments.size() > 1) {
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers((GLsizei)m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.empty()) {
			glDrawBuffer(GL_NONE); // Only Depth-Pass
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER))
			OC_CORE_ERROR("OpenGL framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}	// Ocean::GL

