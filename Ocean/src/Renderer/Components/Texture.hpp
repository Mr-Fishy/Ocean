#pragma once

#include "Ocean/Core/Types/Bool.hpp"
#include "Ocean/Core/Types/Integers.hpp"

namespace Ocean {

	namespace Vulkan {

		class Device;

		class Texture {
		public:
			virtual ~Texture() = default;

			virtual u32 GetWidth() const = 0;
			virtual u32 GetHeight() const = 0;

			virtual void SetData(void* data, u32 size) = 0;

			virtual void Bind(u32 slot = 0) const = 0;

			virtual b8 operator == (const Texture&) const = 0;

		};

		class Texture2D : public Texture {
		public:
			
		};

	}	// Vulkan

}	// Ocean
