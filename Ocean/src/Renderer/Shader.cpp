#include "Shader.hpp"

#include "Ocean/Core/Primitives/Memory.hpp"
#include "Ocean/Core/Primitives/Assert.hpp"

// std
#include <fstream>

namespace Ocean {

	namespace Vulkan {

		Shader::Shader(cstring filename) : m_ActiveModule(false) {
			ReadShaderFile(filename);
		}

		void Shader::Init(Allocator* allocator, cstring filename) {
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

			u32 fileSize = (u32)file.tellg();
			m_ShaderFile.Init(fileSize);

			file.seekg(0);
			file.read(m_ShaderFile.Data(), fileSize);
			m_ShaderFile.SetSize(fileSize);
			

			file.close();
		}

	}	// Vulkan

}	// Ocean
