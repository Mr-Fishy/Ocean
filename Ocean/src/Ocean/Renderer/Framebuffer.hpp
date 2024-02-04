#pragma once

#include "Ocean/Core/Base.hpp"

namespace Ocean {

	// Specifies the Framebuffer properties, i.e. the width and height or the number of samples to save.
	//
	struct FramebufferSpecification {
		uint32_t Width = 0, Height = 0;
		// FramebufferFormat Format = 
		uint32_t Samples = 1;

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

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};

}	// Ocean
