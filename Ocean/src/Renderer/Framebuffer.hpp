#pragma once

#include "Ocean/Core/Types/SharedPtr.hpp"
#include "Ocean/Core/Types/Bool.hpp"
#include "Ocean/Core/Types/Integers.hpp"

// std
#include <initializer_list>
#include <vector>

namespace Ocean {

    namespace Shrimp {

        enum FramebufferFormat {
            None = 0,

            // Color
            RGBA8,
            Red_Int,

            // Depth / Stencil
            Depth24_Stencil8,

            // Defaults
            Depth = Depth24_Stencil8

        };  // FramebufferFormat

        struct FramebufferTextureSpec {
            FramebufferTextureSpec() : textureFormat(FramebufferFormat::None) { }
            FramebufferTextureSpec(FramebufferFormat format) : textureFormat(format) { }

            FramebufferFormat textureFormat;

        };  // FramebufferTextureSpec

        struct FramebufferAttachmentSpec {
            FramebufferAttachmentSpec() : attachments() { }
            FramebufferAttachmentSpec(std::initializer_list<FramebufferTextureSpec> attachments) : attachments(attachments) { }

            std::vector<FramebufferTextureSpec> attachments;

        };  // FramebufferAttachmentSpec

        struct FramebufferSpecification {
            FramebufferSpecification() : width(0), height(0), samples(1), attachments(), swapChainTarget(false) { }

            u32 width, height;

            u32 samples;

            FramebufferAttachmentSpec attachments;

            b8 swapChainTarget;

        };  // FramebufferSpecification



        class Framebuffer {
        public:
            virtual ~Framebuffer() = default;

            virtual void Bind() = 0;
            virtual void Unbind() = 0;

            virtual void Resize(u32 width, u32 height) = 0;
            
            virtual u32 ReadPixel(u32 attachmentIndex, i32 x, i32 y) = 0;

            virtual u32 GetColorAttachmentID(u32 index = 0) const = 0;
            virtual void ClearAttachment(u32 attachmentIndex, i32 value) = 0;

            virtual const FramebufferSpecification& GetSpecification() const = 0;

            static SharedPtr<Framebuffer> Create(const FramebufferSpecification& spec);

        };  // Framebuffer

    }   // Ocean

}   // Ocean
