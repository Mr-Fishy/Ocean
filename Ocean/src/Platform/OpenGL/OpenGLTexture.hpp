#pragma once

#include "Ocean/Renderer/Texture.hpp"

// libs
#include <glad/gl.h>

namespace Ocean::GL {

	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(ui32 width, ui32 height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual ui32 GetWidth() const override final { return m_Width; }
		virtual ui32 GetHeight() const override final { return m_Height; }
		virtual ui32 GetID() const override final { return m_RendererID; }

		virtual void SetData(void* data, ui32 size) override final;

		virtual void Bind(ui32 slot = 0) const override final;

		virtual bool operator == (const Texture& other) const override final;

	private:
		std::string m_Path;

		ui32 m_Width;
		ui32 m_Height;

		ui32 m_RendererID;
		i32 m_InternalFormat, m_DataFormat;
	};

}	// Ocean::GL
