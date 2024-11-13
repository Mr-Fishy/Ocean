#include "Shader.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"

#include "Renderer/Resources.hpp"

// std
#include <fstream>

namespace Ocean {

	namespace Vulkan {

		Shader::Shader(const Shader& shader) : p_DeviceRef(shader.p_DeviceRef), m_ShaderFile(shader.m_ShaderFile), m_Module(shader.m_Module), m_ActiveModule(shader.m_ActiveModule) { }

		Shader::Shader(cstring filename) : p_DeviceRef(VK_NULL_HANDLE), m_ShaderFile(), m_Module(VK_NULL_HANDLE), m_ActiveModule(false) {
			ReadShaderFile(filename);
		}

		Shader& Shader::operator = (const Shader& shader) {
			if (this != &shader) {
				this->p_DeviceRef = shader.p_DeviceRef;
				this->m_ShaderFile = shader.m_ShaderFile;
				this->m_Module = shader.m_Module;
				this->m_ActiveModule = shader.m_ActiveModule;
			}

			return *this;
		}

		void Shader::Init(cstring filename) {
			m_ActiveModule = false;

			ReadShaderFile(filename);
		}

		void Shader::Shutdown() {
			if (m_Module)
				DestroyShader();

			m_ShaderFile.Shutdown();
		}

		VkShaderModule Shader::GetShaderModule(VkDevice device) {
			if (m_ActiveModule)
				return m_Module;

			p_DeviceRef = device;

			VkShaderModuleCreateInfo info{ };
			info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

			info.codeSize = m_ShaderFile.Size();
			info.pCode = reinterpret_cast<const u32*>(m_ShaderFile.Data());

			CHECK_RESULT(
				vkCreateShaderModule(device, &info, nullptr, &m_Module),
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
