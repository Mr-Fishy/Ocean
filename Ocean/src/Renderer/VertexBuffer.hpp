#pragma once

#include "Ocean/Core/Types/Bool.hpp"
#include "Ocean/Core/Types/SmartPtrs.hpp"
#include "Ocean/Core/Types/Strings.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"
#include "Ocean/Core/Primitives/Array.hpp"

// std
#include <initializer_list>

namespace Ocean {

    namespace Shrimp {

        // TODO: Enum to String Name Macro?
        enum ShaderDataType {
            None = 0,

            Int, Int2, Int3, Int4,

            Float, Float2, Float3, Float4,

            Mat3, Mat4,

            Bool,

        };  // ShaderDataType

        inline static u32 ShaderDataTypeSize(ShaderDataType type) {
            switch (type) {
                case ShaderDataType::None:     break;

                case ShaderDataType::Int:      return 4;
                case ShaderDataType::Int2:     return 4 * 2;
                case ShaderDataType::Int3:     return 4 * 3;
                case ShaderDataType::Int4:     return 4 * 4;

                case ShaderDataType::Float:    return 4;
                case ShaderDataType::Float2:   return 4 * 2;
                case ShaderDataType::Float3:   return 4 * 3;
                case ShaderDataType::Float4:   return 4 * 4;

                case ShaderDataType::Mat3:     return 4 * 3 * 3;
                case ShaderDataType::Mat4:     return 4 * 4 * 4;

                case ShaderDataType::Bool:     return 1;
            }

            OASSERTM(false, "Unkown ShaderDataType!");
            return 0;
        }



        struct BufferElement {
            cstring name;

            ShaderDataType type;
            
            u32 size;
            u32 offset;

            b8 normalized;

            BufferElement() = default;
            BufferElement(ShaderDataType type, cstring name, b8 normalized = false)
                : name(name), type(type), size(ShaderDataTypeSize(type)), offset(0), normalized(normalized) { }

            u32 GetComponentCount() const { 
                switch (this->type)
                {
                    case ShaderDataType::None:      break;

                    case ShaderDataType::Float:		return 1;
                    case ShaderDataType::Float2:    return 2;
                    case ShaderDataType::Float3:	return 3;
                    case ShaderDataType::Float4:	return 4;

                    case ShaderDataType::Mat3:		return 3; // 3 * float3
                    case ShaderDataType::Mat4:		return 4; // 4 * float4

                    case ShaderDataType::Int:		return 1;
                    case ShaderDataType::Int2:		return 2;
                    case ShaderDataType::Int3:		return 3;
                    case ShaderDataType::Int4:		return 4;

                    case ShaderDataType::Bool:		return 1;
                }

                OASSERTM(false, "Unkown ShaderDataType!");
                return 0;
            }

        };  // BufferElement

        class BufferLayout {
        public:
            BufferLayout() = default;
            BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements), m_Stride(0) {
                CalculateOffsetsAndStride();
            }
            ~BufferLayout() = default;

            inline u32 GetStride() const { return this->m_Stride; }

            inline const DynamicArray<BufferElement>& GetElements() const { return this->m_Elements; }

            std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
            std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

            std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
            std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

        private:
            inline void CalculateOffsetsAndStride() {
                u32 offset = 0;
                this->m_Stride = 0;

                for (auto& element : this->m_Elements) {
                    element.offset = offset;
                    offset += element.size;
                    this->m_Stride += element.size;
                }
            }

            DynamicArray<BufferElement> m_Elements;

            u32 m_Stride;

        };  // BufferLayout



        class VertexBuffer {
        public:
            virtual ~VertexBuffer() = default;

            virtual void Bind() const = 0;
            virtual void Unbind() const = 0;

            virtual void SetData(const void* data, u32 size) = 0;

            virtual const BufferLayout& GetLayout() const = 0;
            virtual void SetLayout(const BufferLayout& layout) = 0;

            static Ref<VertexBuffer> Create(u32 size);
            static Ref<VertexBuffer> Create(float* vertices, u32 size);

        };  // VertexBuffer

    }   // Shrimp

}   // Ocean