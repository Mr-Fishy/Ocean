#pragma once

/**
 * @file Framebuffer.hpp
 * @author Evan F.
 * @brief The abstract Framebuffer for Ocean.
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Types/SmartPtrs.hpp"
#include "Ocean/Types/Bool.hpp"
#include "Ocean/Types/Integers.hpp"

#include "Ocean/Primitives/Macros.hpp"
#include "Ocean/Primitives/Array.hpp"

namespace Ocean {

    namespace Splash {

        /**
         * @brief An enum of Ocean supported Framebuffer formats.
         */
        enum FramebufferFormat {
            /** @brief Null protection option. */
            None = 0,

            /** @brief Standard 8-bit RGBA color. */
            RGBA8,
            /**
             * @brief 32-bit red color.
             * @details Usefull for storing data (i.e. ID's) in pixels.
             */
            Red_Int,

            /** @brief 24-bit depth pixel data combined with 8-bit stencil data. */
            Depth24_Stencil8,

            /** @brief The default Depth format. */
            Depth = Depth24_Stencil8

        };  // FramebufferFormat

        /**
         * @brief A struct that holds the information related to the texture of the Framebuffer.
         *
         * @details AKA the FramebufferFormat.
         */
        struct FramebufferTextureSpec {
            OC_INLINE FramebufferTextureSpec() : textureFormat(FramebufferFormat::None) { }
            /**
             * @brief Construct a new Framebuffer Texture Spec object.
             * 
             * @param format The FramebufferFormat to set.
             */
            OC_INLINE FramebufferTextureSpec(FramebufferFormat format) : textureFormat(format) { }

            FramebufferFormat textureFormat; /** @brief The format of the Framebuffer texture, represented as a FramebufferFormat. */

        };  // FramebufferTextureSpec

        /**
         * @brief A struct that holds the information related to the attachments for the Framebuffer.
         * 
         * @details AKA the list of FramebufferTextureSpec's.
         */
        struct FramebufferAttachmentSpec {
            OC_INLINE FramebufferAttachmentSpec() : specs() { }
            /**
             * @brief Construct a new Framebuffer Attachment Spec object with the given attachments.
             * 
             * @param specs The FramebufferTextureSpec's to use as attachment specifications.
             */
            OC_INLINE FramebufferAttachmentSpec(std::initializer_list<FramebufferTextureSpec> specs) : specs(specs) { }

            DynamicArray<FramebufferTextureSpec> specs; /** @brief A list of FramebufferTextureSpec's that define's the Framebuffer's texture layers. */

        };  // FramebufferAttachmentSpec

        /**
         * @brief A struct that holds all of the information required to create a Framebuffer.
         */
        struct FramebufferSpecification {
            OC_INLINE FramebufferSpecification() : width(0), height(0), samples(1), attachments(), swapChainTarget(false) { }

            u32 width; /** @brief The width of the Framebuffer. */
            u32 height; /** @brief The height of the Framebuffer. */

            u32 samples; /** @brief The number of samples to run on each attachment. */

            FramebufferAttachmentSpec attachments; /** @brief The FramebufferAttachmentSpec's that define each texture attachment. */

            b8 swapChainTarget; /** @brief Records if the Framebuffer is to be used in a swapchain by the Window. */

        };  // FramebufferSpecification



        /**
         * @brief The Framebuffer controls the framebuffer image through the renderer API.
         */
        class Framebuffer {
        public:
            /**
             * @brief Construct a new Framebuffer object with the given specification.
             * 
             * @param spec The FramebufferSpecification to use.
             */
            Framebuffer(const FramebufferSpecification& spec);
            virtual ~Framebuffer() = default;

            /**
             * @brief Bind's the Framebuffer for use by commands.
             */
            virtual void Bind() = 0;
            /**
             * @brief Unbind's the Framebuffer to not be available for use by commands.
             */
            virtual void Unbind() = 0;

            /**
             * @brief Marks the Framebuffer as invalid and recreates the Framebuffer.
             */
            virtual void Invalidate() = 0;

            /**
             * @brief Resize the Framebuffer to the new width and height.
             * 
             * @param width The new width to set the Framebuffer.
             * @param height The new height to set the Framebuffer.
             */
            virtual void Resize(u32 width, u32 height) = 0;
            
            /**
             * @brief Read a pixel for pixel data at the given position on the given attachment.
             * 
             * @param attachmentIndex The index of the attachment to read from.
             * @param x The x position to read from.
             * @param y The y position to read from.
             * @return u32 
             */
            virtual u32 ReadPixel(u32 attachmentIndex, i32 x, i32 y) = 0;

            /**
             * @brief Get the ID of the color attachment from the given index.
             * 
             * @param index The index of the color attachment.
             * @return u32 
             */
            virtual u32 GetColorAttachmentID(u32 index = 0) const = 0;
            /**
             * @brief Clear the attachment at the given index and set it to the given value.
             * 
             * @param attachmentIndex The index of the attachment to clear.
             * @param value The value to set the attachment image.
             */
            virtual void ClearAttachment(u32 attachmentIndex, i32 value) = 0;

            /**
             * @brief Get the FramebufferSpecification from the Framebuffer.
             * 
             * @return const FramebufferSpecification& 
             */
            const FramebufferSpecification& GetSpecification() const { return this->m_Specification; }

            /**
             * @brief Create a new Framebuffer with the given FramebufferSpecification.
             * 
             * @param spec The FramebufferSpecification to use.
             * @return Ref<Framebuffer> 
             */
            OC_STATIC Ref<Framebuffer> Create(const FramebufferSpecification& spec);

        protected:
            OC_INLINE b8 IsDepthFormat(FramebufferFormat format) {
                switch (format) {
                    case FramebufferFormat::None:                
                    case FramebufferFormat::RGBA8:                
                    case FramebufferFormat::Red_Int:
                        break;
                    
                    case FramebufferFormat::Depth24_Stencil8:
                        return true;
                }

                return false;
            }

        protected:
            FramebufferSpecification m_Specification; /** @brief The top-level FramebufferSpecification. */

            DynamicArray<FramebufferTextureSpec> m_ColorAttachmentSpecs; /** @brief The top-level color attachment texture specifications. */
            FramebufferTextureSpec m_DepthAttachmentSpec; /** @brief The top-level depth attachment texture specification. */

        };  // Framebuffer

    }   // Ocean

}   // Ocean
