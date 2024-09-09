#pragma once

#include "Ocean/Core/Types/Strings.hpp"
#include "Ocean/Core/Primitives/Array.hpp"

#include "Renderer/VulkanResources.hpp"

namespace Ocean {

	class Allocator;

	namespace Vulkan {

		class Shader {
		public:
			void Init(Allocator* allocator, cstring filename);
			void Shutdown();

			VkShaderModule GetShaderModule(VkDevice device);
			void DestroyShader(VkDevice device);

		private:
			void ReadShaderFile(cstring filename);

			/* --- */

			Allocator* p_Allocator = nullptr;

			DynamicArray<char> m_ShaderFile;

			VkShaderModule m_Module = VK_NULL_HANDLE;
		};

	}	// Vulkan

}	// Ocean
