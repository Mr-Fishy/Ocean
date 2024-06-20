#include "ocpch.hpp"

#include "Platform/OpenGL/OpenGLShader.hpp"

#include "Ocean/Core/Log.hpp"

// libs
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

// std
#include <fstream>

namespace Ocean::GL {

	namespace Internal {

		static GLenum ShaderTypeFromString(const std::string& type) {
			if (type == "vertex")
				return GL_VERTEX_SHADER;
			if (type == "fragment")
				return GL_FRAGMENT_SHADER;

			OC_CORE_ERROR("Unkown OpenGL Shader Type!");
			return 0;
		}

	}	// Internal

	OpenGLShader::OpenGLShader(const std::string& file) {
		std::string source = ReadFile(file);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// Get Name From Filepath
		auto lastSlash = file.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = file.rfind('.');
		auto count = lastDot == std::string::npos ? file.size() - lastSlash : lastDot - lastSlash;
		m_Name = file.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragSrc) : m_Name(name) {
		std::unordered_map<i32, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragSrc;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const {
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const {
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, i32 value) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());

		glUniform1i(location, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, i32* values, ui8 count) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());

		glUniform1iv(location, count, values);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());

		glUniform1f(location, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const vec2& value) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());

		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const vec3& value) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());

		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const vec4& value) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());

		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetMat3(const std::string& name, const mat3& value) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());

		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetMat4(const std::string& name, const mat4& value) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());

		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	std::string OpenGLShader::ReadFile(const std::string& file) {
		std::string result;
		std::ifstream in(file, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII

		if (in) {
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();

			if (size != -1) {
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
				in.close();
			}
			else {
				OC_CORE_ERROR("Could not read from file: " + file);
			}
		}
		else {
			OC_CORE_ERROR("Unable to open file at: " + file);
		}

		return result;
	}

	std::unordered_map<i32, std::string> OpenGLShader::PreProcess(const std::string& source) {
		std::unordered_map<i32, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);					// Start of Shader Type Declaration Line
		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);		// End of Shader Type Declaration Line
			if (eol == std::string::npos)
				OC_CORE_ERROR("OpenGL::PreProcess :| Shader pre-processing error!");

			size_t begin = pos + typeTokenLength + 1;			// Start of Shader Type Name (After "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			if (!Internal::ShaderTypeFromString(type))
				OC_CORE_ERROR("OpenGL::PreProcess :| Invalid shader type specified!");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);		// Start of Shader Code After Shader Type Declaration Line
			if (nextLinePos == std::string::npos)
				OC_CORE_ERROR("OpenGL::PreProcess :| Shader pre-processing error!");

			pos = source.find(typeToken, nextLinePos);			// Start of Next Shader Type Declaration Line
			shaderSources[Internal::ShaderTypeFromString(type)] =
				(pos == std::string::npos) ? source.substr(nextLinePos)
				: source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(std::unordered_map<i32, std::string>& shaderSources) {
		// Adapted from provided code on OpenGL site
		GLuint program = glCreateProgram();
		// OC_CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now :(");

		std::array<GLenum, 2> glShaderIDs{};
		i32 glShaderIDIndex = 0;

		for (auto& kv : shaderSources) {
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE) {
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				OC_CORE_ERROR(infoLog.data());
				// OC_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);

			glShaderIDs[glShaderIDIndex++] = shader;
		}

		m_RendererID = program;

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			for (auto id : glShaderIDs) {
				glDeleteShader(id);
			}

			OC_CORE_ERROR(infoLog.data());
			// OC_CORE_ASSERT(false, "OpenGLShader link failure!");
			return;
		}

		for (auto id : glShaderIDs) {
			glDetachShader(program, id);
		}
	}

}	// Ocean::GL
