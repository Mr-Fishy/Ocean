#pragma once

#include "Ocean/Renderer/Texture.hpp"

// libs
#include <vulkan/vulkan.hpp>

namespace Ocean::Vk {

	class VulkanTexture2D : public Texture2D {
	public:
		VulkanTexture2D(ui32 width, ui32 height);
		VulkanTexture2D(const std::string& path);
		virtual ~VulkanTexture2D();

		virtual ui32 GetWidth() const override final { return m_Width; }
		virtual ui32 GetHeight() const override final { return m_Height; }
		virtual ui32 GetID() const override final { return ui32(); }

		virtual void SetData(void* data, ui32 size) override final;

		virtual void Bind(ui32 slot = 0) const override final;

		virtual bool operator == (const Texture& other) const override final;

	private:
		std::string m_Path;

		ui32 m_Width, m_Height;

		// Vulkan Texture Format
	};

}	// Ocean::Vk
