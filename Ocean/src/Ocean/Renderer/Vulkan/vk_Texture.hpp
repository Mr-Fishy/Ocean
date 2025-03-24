#pragma once

/**
 * @file vk_Texture.hpp
 * @author Evan F.
 * @brief 
 * @date 2025-01-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Renderer/Texture.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {

        class vkTexture2D : public Texture2D {
        public:
            vkTexture2D(u32 width, u32 height);
            virtual ~vkTexture2D();

            virtual void Bind(u32 slot) const override final;

            virtual void SetData(void* data, u32 size) override final;
            virtual void SetFormat(TextureFormat format) override final;

            inline virtual u32 GetWidth() const { return this->m_Width; }
            inline virtual u32 GetHeight() const { return this->m_Height; }

            inline virtual u32 GetRendererID() const override final { return 0; }

            inline virtual bool operator == (const Texture& other) const override final { return this->m_Image == static_cast<const vkTexture2D&>(other).m_Image; }

        private:
            VkImage m_Image;
            VkImageLayout m_Layout;

            VkSampler m_Sampler;

            VkDeviceMemory m_Memory;
            VkImageView m_View;

            i32 m_Width;
            i32 m_Height;

        };  // Texture2D

    }   // Splash

}   // Ocean
