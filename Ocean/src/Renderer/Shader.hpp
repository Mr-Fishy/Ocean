#pragma once

#include "Ocean/Core/Types/Strings.hpp"

#include "Ocean/Core/Primitives/Array.hpp"

// libs
#include <vulkan/vulkan.h>

namespace Ocean {

	class Allocator;

	namespace Vulkan {

		// TODO: Make a shader contain both the fragment and vertex components of the shader.
		// TODO: WTF are SPER-V shaders? How work?
		// TODO: Native hardcoded shaders that are always available (ie standard geometry with textures, text and ui).
		/**
		 * @brief A Shader contains the file(s) for a fragment or vertex component, and creates the shader module when requested.
		 */
		class Shader {
		public:
			Shader() : p_DeviceRef(VK_NULL_HANDLE), m_ShaderFile(), m_Module(VK_NULL_HANDLE), m_ActiveModule(false) { }
			Shader(const Shader& shader);
			Shader(cstring filename);
			~Shader() = default;

			Shader& operator = (const Shader&);

			/**
			 * @brief Initializes the Shader to use the given allocator and filename.
			 * @param filename - The file to load the shader from. Loading from the src directory of the application.
			 */
			void Init(cstring filename);
			/**
			 * @brief Shuts down the shader, if the shader module has not been destroyed, it will destroy it.
			 */
			void Shutdown();

			/**
			 * @param device - Vulkan Device to get the shader module from (in most cases this will be the same device as in Ocean::Device).
			 * @return The Vulkan Shader Module necessary for building the Vulkan Graphics Pipeline.
			 */
			VkShaderModule GetShaderModule(VkDevice device);
			/**
			 * @brief Destroy's the Vulkan Shader Module.
			 */
			void DestroyShader();

		private:
			/**
			 * @brief Load's the shader file into an array.
			 * @param filename - The filename (path included) of the shader file to load.
			 */
			void ReadShaderFile(cstring filename);

			/* --- */

			VkDevice p_DeviceRef;

			DynamicArray<char> m_ShaderFile;

			VkShaderModule m_Module;
			b8 m_ActiveModule;
		};

	}	// Vulkan

}	// Ocean
