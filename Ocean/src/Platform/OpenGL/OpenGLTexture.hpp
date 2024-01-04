#pragma once

#include "Ocean/Renderer/Texture.hpp"

// libs
#include <glad/gl.h>

namespace Ocean {

	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const { return m_Width; }
		virtual uint32_t GetHeight() const { return m_Height; }
		virtual uint32_t GetRendererID() const { return m_RendererID; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot) const override;

		virtual bool operator==(const Texture& other) const override { return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID; }

	private:
		std::string m_Path;

		uint32_t m_Width;
		uint32_t m_Height;

		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};

}	// Ocean