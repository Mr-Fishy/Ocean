#pragma once

#include "Ocean/Core/Types.hpp"

// std
#include <string>

namespace Ocean {

	class Texture {
	public:
		virtual ~Texture() = default;

		virtual ui32 GetWidth() const = 0;
		virtual ui32 GetHeight() const = 0;
		virtual ui32 GetID() const = 0;

		virtual void SetData(void* data, ui32 size) = 0;

		virtual void Bind(ui32 slot = 0) const = 0;

		virtual bool operator == (const Texture& other) const = 0;
	};

	class Texture2D : public Texture {
	public:
		static Ref<Texture2D> Create(ui32 width, ui32 height);
		static Ref<Texture2D> Create(const std::string& path);
	};

}	// Ocean
