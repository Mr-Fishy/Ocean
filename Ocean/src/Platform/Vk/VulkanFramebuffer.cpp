#include "ocpch.hpp"

#include "Platform/Vk/VulkanFramebuffer.hpp"

namespace Ocean::Vk {

	VulkanFramebuffer::VulkanFramebuffer(const FramebufferSpec& spec) {
	}

	VulkanFramebuffer::~VulkanFramebuffer() {
	}

	void VulkanFramebuffer::Invalidate() {
	}

	void VulkanFramebuffer::Bind() {
	}

	void VulkanFramebuffer::Unbind() {
	}

	void VulkanFramebuffer::Resize(ui32 width, ui32 height) {
	}

	int VulkanFramebuffer::ReadPixel(ui32 attachmentIndex, i32 x, i32 y) {
		return 0;
	}

	ui32 VulkanFramebuffer::GetColorAttachmentID(ui32 index) const {
		return ui32();
	}

	void VulkanFramebuffer::ClearAttachment(ui32 attachmentIndex, i32 value) {
	}

}	// Ocean::Vk
