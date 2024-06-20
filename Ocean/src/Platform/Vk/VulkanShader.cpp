#include "ocpch.hpp"

#include "Platform/Vk/VulkanShader.hpp"

namespace Ocean::Vk {

	VulkanShader::VulkanShader(const std::string& file) {
	}

	VulkanShader::VulkanShader(const std::string& name, const std::string& vertexSrc, const std::string& fragSrc) {
	}

	VulkanShader::~VulkanShader() {
	}

	void VulkanShader::Bind() const {
	}

	void VulkanShader::Unbind() const {
	}

	void VulkanShader::SetInt(const std::string& name, i32 value) {
	}

	void VulkanShader::SetIntArray(const std::string& name, i32* values, ui8 count) {
	}

	void VulkanShader::SetFloat(const std::string& name, float value) {
	}

	void VulkanShader::SetFloat2(const std::string& name, const vec2& value) {
	}

	void VulkanShader::SetFloat3(const std::string& name, const vec3& value) {
	}

	void VulkanShader::SetFloat4(const std::string& name, const vec4& value) {
	}

	void VulkanShader::SetMat3(const std::string& name, const mat3& value) {
	}

	void VulkanShader::SetMat4(const std::string& name, const mat4& value) {
	}

	std::string VulkanShader::ReadFile(const std::string& file) {
		return std::string();
	}

	std::unordered_map<ui8, std::string> VulkanShader::PreProcess(const std::string& source) {
		return std::unordered_map<ui8, std::string>();
	}

	void VulkanShader::Compile(std::unordered_map<ui8, std::string>& shaderSources) {
	}

}	// Ocean::Vk
