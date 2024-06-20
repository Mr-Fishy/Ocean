#pragma once

#include "Ocean/Renderer/Shader.hpp"

namespace Ocean::Vk {

	class VulkanShader : public Shader {
	public:
		VulkanShader(const std::string& file);
		VulkanShader(const std::string& name, const std::string& vertexSrc, const std::string& fragSrc);
		virtual ~VulkanShader();


		virtual void Bind() const override final;
		virtual void Unbind() const override final;

		virtual void SetInt(const std::string& name, i32 value) override final;
		virtual void SetIntArray(const std::string& name, i32* values, ui8 count) override final;

		virtual void SetFloat(const std::string& name, float value) override final;
		virtual void SetFloat2(const std::string& name, const vec2& value) override final;
		virtual void SetFloat3(const std::string& name, const vec3& value) override final;
		virtual void SetFloat4(const std::string& name, const vec4& value) override final;

		virtual void SetMat3(const std::string& name, const mat3& value) override final;
		virtual void SetMat4(const std::string& name, const mat4& value) override final;

		virtual const std::string& GetName() const override final { return m_Name; }

	private:
		std::string ReadFile(const std::string& file);
		
		std::unordered_map<ui8, std::string> PreProcess(const std::string& source);

		void Compile(std::unordered_map<ui8, std::string>& shaderSources);

		/* --- */

		std::string m_Name;
	};

}	// Ocean::Vk
