#include "gl_IndexBuffer.hpp"

// libs
#include <glad/gl.h>

namespace Ocean {

    namespace Shrimp {
    
        glIndexBuffer::glIndexBuffer(u32* indices, u32 count) : m_RendererID(0), m_Count(count) {
            glCreateBuffers(1, &this->m_RendererID);

            glBindBuffer(GL_ARRAY_BUFFER, this->m_RendererID);
            glBufferData(GL_ARRAY_BUFFER, count * sizeof(u32), indices, GL_STATIC_DRAW);
        }

        glIndexBuffer::~glIndexBuffer() {
            glDeleteBuffers(1, &this->m_RendererID);
        }

        void glIndexBuffer::Bind() const {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_RendererID);
        }

        void glIndexBuffer::Unbind() const {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

    }   // Shrimp

}   // Ocean