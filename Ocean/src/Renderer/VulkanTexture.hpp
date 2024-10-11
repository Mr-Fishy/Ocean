#pragma once

namespace Ocean {

	namespace Vulkan {

		class Device;

		class Texture {
		public:
			void UpdateDescriptor();
			void Destroy();

		};

		class Texture2D : public Texture {

		};

	}	// Vulkan

}	// Ocean
