#include "gl_VertexBuffer.hpp"

// libs
#include <glad/gl.h>

namespace Ocean {

    namespace Shrimp {

        glVertexBuffer::glVertexBuffer(u32 size) {
            glCreateBuffers(1, &this->m_RendererID);
            glBindBuffer(GL_ARRAY_BUFFER, this->m_RendererID);
            glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
        }

        glVertexBuffer::glVertexBuffer(f32* vertices, u32 size) {
            glCreateBuffers(1, &this->m_RendererID);
            glBindBuffer(GL_ARRAY_BUFFER, this->m_RendererID);
            glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        }

        glVertexBuffer::~glVertexBuffer() {
            glDeleteBuffers(1, &this->m_RendererID);
        }

        void glVertexBuffer::Bind() const {
            glBindBuffer(GL_ARRAY_BUFFER, this->m_RendererID);
        }

        void glVertexBuffer::Unbind() const {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        void glVertexBuffer::SetData(const void* data, u32 size) {
            glBindBuffer(GL_ARRAY_BUFFER, this->m_RendererID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
        }

    }   // Shrimp

}   // Ocean
