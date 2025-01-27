#pragma once

/**
 * @file vk_VertexArray.hpp
 * @author Evan F.
 * @brief The Vulkan implementation of a @ref VertexArray.
 * @date 2025-01-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Renderer/IndexBuffer.hpp"
#include "Ocean/Renderer/VertexArray.hpp"
#include "Ocean/Renderer/VertexBuffer.hpp"

namespace Ocean {

    namespace Splash {

        class vkVertexArray : public VertexArray {
        public:
            vkVertexArray();
            ~vkVertexArray();

            virtual void Bind() const override final;
            virtual void Unbind() const override final;

            virtual void AddVertexBuffer(const Ref<VertexBuffer>& buffer) override final;
            inline virtual const DynamicArray<Ref<VertexBuffer>>& GetVertexBuffers() const override final { return m_VertexBuffers; }

            virtual void SetIndexBuffer(const Ref<IndexBuffer>& buffer) override final;
            inline virtual const Ref<IndexBuffer>& GetIndexBuffer() const override final { return m_IndexBuffer; }

        private:
            // u32 m_RendererID;

            // u32 m_VertexBufferIndex;

            DynamicArray<Ref<VertexBuffer>> m_VertexBuffers;
            Ref<IndexBuffer> m_IndexBuffer;

        };  // VertexArray

    }   // Splash

}   // Ocean
