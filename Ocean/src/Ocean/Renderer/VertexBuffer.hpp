#pragma once

/**
 * @file VertexBuffer.hpp
 * @author Evan F.
 * @brief The abstract VertexBuffer for Ocean.
 * @date 01-14-2025
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Ocean/Types/Bool.hpp"
#include "Ocean/Types/SmartPtrs.hpp"
#include "Ocean/Types/Strings.hpp"

#include "Ocean/Primitives/Assert.hpp"
#include "Ocean/Primitives/Array.hpp"

// std
#include <initializer_list>

namespace Ocean {

    namespace Shrimp {

        /**
         * @brief Enum of the types of data in a Shader.
         */
        enum ShaderDataType {
            /** @brief Null protection option. */
            None = 0,

            /** @brief Single 4-byte integer. */
            Int,
            /** @brief Two-component vector of 4-byte integers. */
            Int2,
            /** @brief Three-component vector of 4-byte integers */
            Int3,
            /** @brief Four-component vector of 4-byte integers */
            Int4,

            /** @brief Single 4-byte float. */
            Float,
            /** @brief Two-component vector of 4-byte floats. */
            Float2,
            /** @brief Three-component vector of 4-byte floats */
            Float3,
            /** @brief Four-component vector of 4-byte floats */
            Float4,

            /** @brief A 3x3 matrix of 4-byte floats. */
            Mat3,
            /** @brief A 4x4 matrix of 4-byte floats. */
            Mat4,

            /** @brief Single 1-byte boolean. */
            Bool,

        };  // ShaderDataType

        /**
         * @brief Gets the size of the given ShaderDataType in bytes.
         * 
         * @param type The type of data to get the size of.
         * @return u8
         */
        inline static u8 ShaderDataTypeSize(ShaderDataType type) {
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



        /**
         * @brief A singular element of data to be used in a VertexBuffer.
         */
        struct BufferElement {
            cstring name; /** @brief The name of the element. */

            ShaderDataType type; /** @brief The type of the element. */
            
            u32 size; /** @brief The size of the element. */
            u32 offset; /** @brief The offset of the element. */

            b8 normalized; /** @brief Records if the element is normalized. */

            BufferElement() = default;
            /**
             * @brief Construct a new BufferElement object.
             * 
             * @param type The element ShaderDataType.
             * @param name The name of the element.
             * @param normalized True if the element is normalized, false otherwise. (OPTIONAL)
             */
            BufferElement(ShaderDataType type, cstring name, b8 normalized = false)
                : name(name), type(type), size(ShaderDataTypeSize(type)), offset(0), normalized(normalized) { }

            /**
             * @brief Get the component count of the element.
             * 
             * @return u8
             */
            u8 GetComponentCount() const { 
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

        /**
         * @brief A collection of BufferElement's to represent a layout.
         */
        class BufferLayout {
        public:
            BufferLayout() : m_Elements(), m_Stride(0) { }
            /**
             * @brief Construct a new BufferLayout object with the given elements.
             * 
             * @param elements A list of elements for the BufferLayout.
             */
            BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements), m_Stride(0) {
                CalculateOffsetsAndStride();
            }
            ~BufferLayout() = default;

            /**
             * @brief Get the stride of the BufferLayout.
             * 
             * @return u32 
             */
            inline u32 GetStride() const { return this->m_Stride; }

            /**
             * @brief Get the list of BufferElements in the layout.
             * 
             * @return const DynamicArray<BufferElement>& 
             */
            inline const DynamicArray<BufferElement>& GetElements() const { return this->m_Elements; }

            std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
            std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

            std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
            std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

        private:
            /**
             * @brief Calculates the element offsets and stride of the layout.
             * 
             */
            inline void CalculateOffsetsAndStride() {
                u32 offset = 0;
                this->m_Stride = 0;

                for (auto& element : this->m_Elements) {
                    element.offset = offset;
                    offset += element.size;
                    this->m_Stride += element.size;
                }
            }

            DynamicArray<BufferElement> m_Elements; /** @brief The list of elements in the layout. */

            /**
             * @brief The stride of the layout.
             * @details Stride simply means the length of the BufferLayout in memory.
             */
            u32 m_Stride;

        };  // BufferLayout



        /**
         * @brief The VertexBuffer controls an array of buffer data through the renderer API.
         */
        class VertexBuffer {
        public:
            virtual ~VertexBuffer() = default;

            /**
             * @brief Bind's the VertexBuffer to be used by commmands.
             */
            virtual void Bind() const = 0;
            /**
             * @brief Unbind's the VertexBuffer from being able to be used by commands.
             */
            virtual void Unbind() const = 0;

            /**
             * @brief Sets the VertexBuffer data given an array pointer and size.
             * 
             * @param data The data to copy.
             * @param size The size of the data array.
             */
            virtual void SetData(const void* data, u32 size) = 0;

            /**
             * @brief Get the BufferLayout of the VertexBuffer.
             * 
             * @return const BufferLayout& 
             */
            virtual const BufferLayout& GetLayout() const = 0;
            /**
             * @brief Set the BufferLayout of the VertexBuffer.
             * 
             * @param layout The new BufferLayout.
             */
            virtual void SetLayout(const BufferLayout& layout) = 0;

            /**
             * @brief Create's a VertexBuffer of a given size.
             * 
             * @param size The size of the VertexBuffer.
             * @return Ref<VertexBuffer> 
             */
            static Ref<VertexBuffer> Create(u32 size);
            /**
             * @brief Create's a VertexBuffer with the given vertices.
             * 
             * @param vertices The array of vertex data as a pointer.
             * @param size The size of the vertices array.
             * @return Ref<VertexBuffer> 
             */
            static Ref<VertexBuffer> Create(float* vertices, u32 size);

        };  // VertexBuffer

    }   // Shrimp

}   // Ocean
