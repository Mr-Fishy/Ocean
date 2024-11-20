#include "Shader.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"

#include "Renderer/Components/VkTypes.hpp"

#include "Renderer/Resources.hpp"

// std
#include <fstream>

namespace Ocean {

	namespace Vulkan {

		Shader::Shader(cstring filename, VkDevice deviceRef) : p_DeviceRef(deviceRef), m_ShaderFile(), m_Module(VK_NULL_HANDLE), m_ActiveModule(false) {
			ReadShaderFile(filename);
		}

		void Shader::Init(cstring filename, VkDevice deviceRef) {
			m_ActiveModule = false;
			p_DeviceRef = deviceRef;

			ReadShaderFile(filename);
		}

		void Shader::Shutdown() {
			if (m_Module)
				DestroyShader();

			m_ShaderFile.Shutdown();
		}

		VkShaderModule Shader::GetShaderModule() {
			if (m_ActiveModule)
				return m_Module;

			Shaders::ModuleCreateInfo createInfo(
				m_ShaderFile.Size(),
				m_ShaderFile.Data()
			);

			CHECK_RESULT(
				vkCreateShaderModule(p_DeviceRef, &createInfo, nullptr, &m_Module),
				"Failed to create shader module!"
			);

			m_ActiveModule = true;
			return m_Module;
		}

		void Shader::DestroyShader() {
			vkDestroyShaderModule(p_DeviceRef, m_Module, nullptr);

			m_Module = nullptr;
		}

		void Shader::ReadShaderFile(cstring filename) {
			std::ifstream file(filename, std::ios::ate | std::ios::binary);

			OASSERTM(file.is_open(), "Failed to open file %s!\n", filename);

			u32 fileSize = static_cast<u32>(file.tellg());
			m_ShaderFile.Init(fileSize);

			file.seekg(0);
			file.read(m_ShaderFile.Data(), fileSize);
			m_ShaderFile.SetSize(fileSize);
			

			file.close();
		}

	}	// Vulkan

}	// Ocean
