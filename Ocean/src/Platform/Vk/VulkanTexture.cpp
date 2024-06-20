#include "ocpch.hpp"

#include "Platform/Vk/VulkanTexture.hpp"

namespace Ocean::Vk {

	VulkanTexture2D::VulkanTexture2D(ui32 width, ui32 height) : m_Width(width), m_Height(height) {

	}

	VulkanTexture2D::VulkanTexture2D(const std::string& path) : m_Width(0), m_Height(0) {

	}

	VulkanTexture2D::~VulkanTexture2D() {

	}

	void VulkanTexture2D::SetData(void* data, ui32 size) {

	}

	void VulkanTexture2D::Bind(ui32 slot) const {

	}

	bool VulkanTexture2D::operator == (const Texture& other) const {
		return false;
	}

}	// Ocean::Vk

