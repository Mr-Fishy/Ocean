#pragma once

#include "Ocean/Core/Base.hpp"

namespace Ocean {

	enum class FramebufferFormat {
		None = 0,
		// Color
		RGBA8,
		RED_INTEGER,

		// Depth / Stencil
		DEPTH24STENCIL8,
		
		// Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification {
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferFormat format) : TextureFormat(format) { }

		FramebufferFormat TextureFormat = FramebufferFormat::None;

		// TODO: Filtering / Wrapping
	};

	struct FramebufferAttachmentSpecification {
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments) : Attachments(attachments) { }

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	// Specifies the Framebuffer properties, i.e. the width and height or the number of samples to save.
	//
	struct FramebufferSpecification {
		uint32_t Width = 0, Height = 0;

		uint32_t Samples = 1;
		FramebufferAttachmentSpecification Attachments;
		bool SwapChainTarget = false;
	};

	// The Framebuffer manages saving the rendered images into memory to be viewed as a texture. This is useful when the frame is not going to be rendered directly to the window.
	//
	class Framebuffer {
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachentIndex, int x, int y) = 0;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};

}	// Ocean
