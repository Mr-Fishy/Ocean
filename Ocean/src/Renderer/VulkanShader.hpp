#pragma once

#include "Ocean/Core/Types/Strings.hpp"
#include "Ocean/Core/Primitives/Array.hpp"

#include "Renderer/VulkanResources.hpp"

namespace Ocean {

	class Allocator;

	namespace Vulkan {

		class Shader {
		public:
			Shader() = default;
			Shader(cstring filename);
			~Shader() = default;

			void Init(Allocator* allocator, cstring filename);
			void Shutdown();

			VkShaderModule GetShaderModule(VkDevice device);
			void DestroyShader();

		private:
			void ReadShaderFile(cstring filename);

			/* --- */

			VkDevice p_DeviceRef = VK_NULL_HANDLE;

			DynamicArray<char> m_ShaderFile;

			VkShaderModule m_Module = VK_NULL_HANDLE;
			b8 m_ActiveModule = false;
		};

	}	// Vulkan

}	// Ocean
