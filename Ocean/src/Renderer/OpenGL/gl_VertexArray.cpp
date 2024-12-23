#include "gl_VertexArray.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"
#include "Ocean/Core/Types/SharedPtr.hpp"
#include "Renderer/IndexBuffer.hpp"

// libs
#include <glad/gl.h>

namespace Ocean {

    namespace Shrimp {

        static GLenum ShaderTypeToGLType(ShaderDataType type) {
            switch (type) {
                case ShaderDataType::None:     break;

                case ShaderDataType::Int:      return GL_INT;
                case ShaderDataType::Int2:     return GL_INT;
                case ShaderDataType::Int3:     return GL_INT;
                case ShaderDataType::Int4:     return GL_INT;

                case ShaderDataType::Float:    return GL_FLOAT;
                case ShaderDataType::Float2:   return GL_FLOAT;
                case ShaderDataType::Float3:   return GL_FLOAT;
                case ShaderDataType::Float4:   return GL_FLOAT;

                case ShaderDataType::Mat3:     return GL_FLOAT;
                case ShaderDataType::Mat4:     return GL_FLOAT;
                
                case ShaderDataType::Bool:     return GL_BOOL;
            }

            OASSERTM(false, "Unkown ShaderDataType!");
            return 0;
        }



        glVertexArray::glVertexArray() {
            glCreateVertexArrays(1, &this->m_RendererID);
        }

        glVertexArray::~glVertexArray() {
            glDeleteVertexArrays(1, &this->m_RendererID);
        }

        void glVertexArray::Bind() const {
            glBindVertexArray(this->m_RendererID);
        }

        void glVertexArray::Unbind() const {
            glBindVertexArray(0);
        }

        void glVertexArray::AddVertexBuffer(const SharedPtr<VertexBuffer>& buffer) {
            OASSERTM(buffer->GetLayout().GetElements().size(), "VertexBuffer Has No Layout!");

            glBindVertexArray(this->m_RendererID);
            buffer->Bind();

            const auto& layout = buffer->GetLayout();
            for (const auto& element : layout) {
                switch (element.type) {

                    case None:
                        break;

                    case Int:
                    case Int2:
                    case Int3:
                    case Int4:
                    case Bool:
                    {
                        glEnableVertexAttribArray(this->m_VertexBufferIndex);
                        glVertexAttribIPointer(
                            this->m_VertexBufferIndex,
                            element.GetComponentCount(),
                            ShaderTypeToGLType(element.type),
                            layout.GetStride(),
                            reinterpret_cast<const void*>(element.offset)
                        );

                        this->m_VertexBufferIndex++;
                        break;
                    }

                    case Float:
                    case Float2:
                    case Float3:
                    case Float4:
                    {
                        glEnableVertexAttribArray(this->m_VertexBufferIndex);
                        glVertexAttribPointer(
                            this->m_VertexBufferIndex,
                            element.GetComponentCount(),
                            ShaderTypeToGLType(element.type),
                            element.normalized ? GL_TRUE : GL_FALSE,
                            layout.GetStride(),
                            reinterpret_cast<const void*>(element.offset)
                        );

                        this->m_VertexBufferIndex++;
                        break;
                    }

                    case Mat3:
                    case Mat4:
                    {
                        u8 count = element.GetComponentCount();
                        for (u8 i = 0; i < count; i++) {
                            glEnableVertexAttribArray(this->m_VertexBufferIndex);
                            glVertexAttribPointer(
                                this->m_VertexBufferIndex,
                                count,
                                ShaderTypeToGLType(element.type),
                                element.normalized ? GL_TRUE : GL_FALSE,
                                layout.GetStride(),
                                reinterpret_cast<const void*>(element.offset + sizeof(float) * count * i)
                            );
                            glVertexAttribDivisor(this->m_VertexBufferIndex, 1);

                            this->m_VertexBufferIndex++;
                        }

                        break;
                    }

                    default:
                        OASSERTM(false, "Unkown ShaderDataType!");
                        break;
                }
            }

            this->m_VertexBuffers.push_back(buffer);
        }

        void glVertexArray::SetIndexBuffer(const SharedPtr<IndexBuffer>& buffer) {
            glBindVertexArray(this->m_RendererID);
            buffer->Bind();

            this->m_IndexBuffer = buffer;
        }

    }   // Shrimp

}   // Ocean