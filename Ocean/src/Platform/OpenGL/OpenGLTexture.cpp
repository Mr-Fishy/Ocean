#include "ocpch.hpp"

#include "Platform/OpenGL/OpenGLTexture.hpp"

// libs
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Ocean::GL {

	OpenGLTexture2D::OpenGLTexture2D(ui32 width, ui32 height) : m_Width(width), m_Height(height) {
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) {
		i32 width, height, channels;

		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		// OC_CORE_ASSERT(data, "Failed to load image!");

		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		switch (channels) {
			case 4:	// Alpha Channel Included
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
				break;

			case 3:	// Alpha Channel Excluded
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		// OC_CORE_ASSERT(internalFormat & dataFormat, "Image format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, ui32 size) {
		ui32 bpp = m_DataFormat == GL_RGBA ? 4 : 3;

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(ui32 slot) const {
		glBindTextureUnit(slot, m_RendererID);
	}

	bool OpenGLTexture2D::operator==(const Texture& other) const {
		return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
	}

}	// Ocean::GL
