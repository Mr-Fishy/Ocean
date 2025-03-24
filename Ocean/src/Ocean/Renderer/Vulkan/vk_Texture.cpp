#include "vk_Texture.hpp"
#include "Ocean/Renderer/Texture.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {

        vkTexture2D::vkTexture2D(u32 width, u32 height) : Texture2D(width, height), m_Image(VK_NULL_HANDLE), m_Layout(), m_Sampler(VK_NULL_HANDLE), m_Memory(VK_NULL_HANDLE), m_View(VK_NULL_HANDLE), m_Width(width), m_Height(height) {
            
        }

        vkTexture2D::~vkTexture2D() {
            
        }

        void vkTexture2D::Bind(u32 slot) const {
            
        }

        void vkTexture2D::SetData(void* data, u32 size) {
            
        }

        void vkTexture2D::SetFormat(TextureFormat format) {
            
        }

    }   // Splash

}   // Ocean
