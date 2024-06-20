#pragma once

#include "Ocean/Core/Types.hpp"

namespace Ocean {

	enum class ShaderDataType {
		None = 0,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool
	};

	static ui32 ShaderDataTypeSize(ShaderDataType type) {
		switch (type) {
			case Ocean::ShaderDataType::Float:		return 4;
			case Ocean::ShaderDataType::Float2:		return 4 * 2;
			case Ocean::ShaderDataType::Float3:		return 4 * 3;
			case Ocean::ShaderDataType::Float4:		return 4 * 4;

			case Ocean::ShaderDataType::Mat3:		return 4 * 3 * 3;
			case Ocean::ShaderDataType::Mat4:		return 4 * 4 * 4;

			case Ocean::ShaderDataType::Int:		return 4;
			case Ocean::ShaderDataType::Int2:		return 4 * 2;
			case Ocean::ShaderDataType::Int3:		return 4 * 3;
			case Ocean::ShaderDataType::Int4:		return 4 * 4;

			case Ocean::ShaderDataType::Bool:		return 1;
		}

		throw std::runtime_error("Unknown ShaderDataType!");

		return 0;
	}

	struct BufferElement {
		std::string Name;
		ShaderDataType Type;
		ui32 Size;
		ui64 Offset;
		b8 Normalized;

		BufferElement() = default;
		BufferElement(ShaderDataType type, const std::string& name, b8 normalized = false)
			: Type(type), Name(name), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) { }

		ui32 GetComponentCount() const {
			switch (Type) {
				case Ocean::ShaderDataType::Float:		return 1;
				case Ocean::ShaderDataType::Float2:		return 2;
				case Ocean::ShaderDataType::Float3:		return 3;
				case Ocean::ShaderDataType::Float4:		return 4;

				case Ocean::ShaderDataType::Mat3:		return 3;	// 3 * float3
				case Ocean::ShaderDataType::Mat4:		return 4;	// 4 * float4

				case Ocean::ShaderDataType::Int:		return 1;
				case Ocean::ShaderDataType::Int2:		return 2;
				case Ocean::ShaderDataType::Int3:		return 3;
				case Ocean::ShaderDataType::Int4:		return 4;

				case Ocean::ShaderDataType::Bool:		return 1;
			}

			throw std::runtime_error("Unknown ShaderDataType in BufferElement!");

			return 0;
		}
	};

	class BufferLayout {
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& elements) : v_Elements(elements) {
			CalculateOffsetsAndStride();
		}

		ui32 GetStride() const { return m_Stride; }
		const std::vector<BufferElement>& GetElements() const { return v_Elements; }

		std::vector<BufferElement>::iterator begin() { return v_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return v_Elements.end(); }

		std::vector<BufferElement>::const_iterator begin() const { return v_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return v_Elements.end(); }

	private:
		void CalculateOffsetsAndStride() {
			ui32 offset = 0;
			m_Stride = 0;

			for (auto& element : v_Elements) {
				element.Offset = offset;
				m_Stride = offset += element.Size;
			}
		}

		/* --- */

		std::vector<BufferElement> v_Elements;
		ui32 m_Stride = 0;
	};

	class VertexBuffer {
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		virtual void SetData(const void* data, ui32 size) = 0;

		static Ref<VertexBuffer> Create(ui32 size);
		static Ref<VertexBuffer> Create(float* vertices, ui32 size);
	};

	class IndexBuffer {
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual ui32 GetCount() const = 0;

		static Ref<IndexBuffer> Create(ui32* indices, ui32 count);
	};

}	// Ocean
