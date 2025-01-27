#pragma once

/**
 * @file vk_IndexBuffer.hpp
 * @author Evan F.
 * @brief 
 * @date 2025-01-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Renderer/IndexBuffer.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {
    
        class vkIndexBuffer : public IndexBuffer {
        public:
            vkIndexBuffer(u32* indices, u32 count);
            virtual ~vkIndexBuffer();

            virtual void Bind() const override final;
            virtual void Unbind() const override final;

            inline virtual u32 GetCount() const override final { return this->m_Count; }

        private:
            VkBuffer m_IndexBuffer;
            VkDeviceMemory m_BufferMemory;

            u32 m_Count;

        };

    }   // Splash

}   // Ocean
