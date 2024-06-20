#pragma once

#include "Ocean/Core/Base.hpp"

// std
#include <initializer_list>
#include <vector>

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

	struct FramebufferTextureSpec {
		FramebufferTextureSpec() = default;
		FramebufferTextureSpec(FramebufferFormat format) : TextureFormat(format) { }

		FramebufferFormat TextureFormat = FramebufferFormat::None;
	};

	struct FramebufferAttachmentSpec {
		FramebufferAttachmentSpec() = default;
		FramebufferAttachmentSpec(std::initializer_list<FramebufferTextureSpec> attachments) : Attachments(attachments) { }

		std::vector<FramebufferTextureSpec> Attachments;
	};

	struct FramebufferSpec {
		ui32 Width = 0, Height = 0;

		ui32 Samples = 1;
		FramebufferAttachmentSpec AttachmentSpecs;
		bool SwapChainTarget = false;
	};

	class Framebuffer {
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(ui32 width, ui32 height) = 0;
		virtual int ReadPixel(ui32 attachmentIndex, i32 x, i32 y) = 0;

		virtual ui32 GetColorAttachmentID(ui32 index = 0) const = 0;
		virtual void ClearAttachment(ui32 attachmentIndex, i32 value) = 0;

		virtual const FramebufferSpec& GetSpec() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpec& spec);
	};

}	// Ocean
