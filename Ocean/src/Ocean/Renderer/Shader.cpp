#include "ocpch.hpp"

#include "Ocean/Renderer/Shader.hpp"

#include "Ocean/Renderer/Renderer.hpp"
#include "Platform/OpenGL/OpenGLShader.hpp"
#include "Platform/Vk/VulkanShader.hpp"

namespace Ocean {

	Ref<Shader> Shader::Create(const std::string& file) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateRef<GL::OpenGLShader>(file);

			case RendererAPI::API::Vulkan:
				return CreateRef<Vk::VulkanShader>(file);
		}

		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragSrc) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateRef<GL::OpenGLShader>(name, vertexSrc, fragSrc);

			case RendererAPI::API::Vulkan:
				return CreateRef<Vk::VulkanShader>(name, vertexSrc, fragSrc);
		}

		return nullptr;
	}

	void ShaderLibrary::Add(const std::string& name, Ref<Shader> shader) {
		if (Exists(name))
			OC_CORE_WARN("ShaderLibrary |: Shader was added that already exists! Name:" + name);

		u_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(Ref<Shader> shader) {
		auto& name = shader->GetName();

		Add(name, shader);
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& file) {
		auto shader = Shader::Create(file);

		Add(shader);

		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& file) {
		auto shader = Shader::Create(file);

		Add(name, shader);

		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name) {
		if (!Exists(name))
			OC_CORE_ERROR("ShaderLibrary |: Shader not found! Name: " + name);

		return u_Shaders[name];
	}

	b8 ShaderLibrary::Exists(const std::string& name) const {
		return u_Shaders.find(name) != u_Shaders.end();
	}

}	// Ocean
