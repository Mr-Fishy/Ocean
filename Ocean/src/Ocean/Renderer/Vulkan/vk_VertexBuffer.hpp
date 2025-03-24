#pragma once

/**
 * @file vk_VertexBuffer.hpp
 * @author Evan F.
 * @brief The Vulkan implementation of a VertexBuffer.
 * @date 2025-01-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Types/FloatingPoints.hpp"

#include "Ocean/Renderer/VertexBuffer.hpp"

// libs
#include <glad/vulkan.h>

namespace Ocean {

    namespace Splash {

        /** @todo Create a vkDescriptorSetLayout to translate the BufferLayout. */

        class vkVertexBuffer : public VertexBuffer {
        public:
            vkVertexBuffer(u32 size);
            vkVertexBuffer(f32* vertices, u32 size);
            virtual ~vkVertexBuffer();

            virtual void Bind() const override final;
            virtual void Unbind() const override final;

            virtual void SetData(const void* data, u32 size) override final;

            inline virtual const BufferLayout& GetLayout() const override final { return this->m_Layout; }
            inline virtual void SetLayout(const BufferLayout& layout) override final { this->m_Layout = layout; }

        private:
            VkBuffer m_VertexBuffer;
            VkDeviceMemory m_BufferMemory;

            BufferLayout m_Layout;

        };  // glVertexBuffer

    }   // Splash

}   // Ocean
