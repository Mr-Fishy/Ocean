#pragma once

#include "Ocean/Core/Types.hpp"

// std
#include <string>
#include <unordered_map>

namespace Ocean {

	class Shader {
	private:
		enum ShaderFlags {
			VERTEX_SHADER		= 1 << 1,
			FRAGMENT_SHADER		= 1 << 2,
		};

	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& name, i32 value) = 0;
		virtual void SetIntArray(const std::string& name, i32* values, ui8 count) = 0;
		
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, const vec2& value) = 0;
		virtual void SetFloat3(const std::string& name, const vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const vec4& value) = 0;

		virtual void SetMat3(const std::string& name, const mat3& value) = 0;
		virtual void SetMat4(const std::string& name, const mat4& value) = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& file);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragSrc);
	};

	class ShaderLibrary {
	public:
		void Add(const std::string& name, Ref<Shader> shader);
		void Add(Ref<Shader> shader);

		Ref<Shader> Load(const std::string& file);
		Ref<Shader> Load(const std::string& name, const std::string& file);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;

	private:
		std::unordered_map<std::string, Ref<Shader>> u_Shaders;
	};

}	// Ocean
