
#ifndef STONECOLD_SHADER_H
#define STONECOLD_SHADER_H

#include <map>
#include <string>
#include <iostream>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "StoneColdBase.hpp"

namespace StoneCold::Resources {

	class Shader {
	public:
		Shader() : _programId(0), _attributes(std::map<std::string, int>()), _uniforms(std::map<std::string, int>()), _success(0), _infoLog("") {};

		// Select (bind/unbind) a buffer to work on its data, layout or VAO assignment
		inline void Bind() const { glUseProgram(_programId); }
		inline void Unbind() const { glUseProgram(0); }

		// Apply basic uniform variables
		inline void SetUniformMat4(const std::string& name, const glm::mat4& mat) const { glUniformMatrix4fv(_uniforms.at(name), 1, false, &mat[0][0]); }
		inline void SetUniformVec4(const std::string& name, const glm::vec4& vec) const { glUniform4fv(_uniforms.at(name), 1, &vec[0]); }
		inline void SetUniformVec3(const std::string& name, const glm::vec3& vec) const { glUniform3fv(_uniforms.at(name), 1, &vec[0]); }
		inline void SetUniformVec2(const std::string& name, const glm::vec2& vec) const { glUniform2fv(_uniforms.at(name), 1, &vec[0]); }
		inline void SetUniformFloat(const std::string& name, float val) const { glUniform1f(_uniforms.at(name), val); }
		inline void SetUniformInt(const std::string& name, int val) const { glUniform1i(_uniforms.at(name), val); }

		virtual ~Shader() { glDeleteProgram(_programId); }

	protected:
		bool CreateShaderProgramm(const std::string& vertex, const std::string& fragment) {
			// Create the basic shaders (from Resource strings)
			auto vertexShader = CreateShader(GL_VERTEX_SHADER, vertex.c_str());
			auto fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragment.c_str());

			// Set up the overall shader program
			_programId = glCreateProgram();
			glAttachShader(_programId, vertexShader);
			glAttachShader(_programId, fragmentShader);
			// Link the program and check for errors
			glLinkProgram(_programId);
			glValidateProgram(_programId);
			glGetProgramiv(_programId, GL_LINK_STATUS, &_success);
			if (_success == GL_FALSE) {
				glGetProgramInfoLog(_programId, 1024, NULL, _infoLog);
				std::cout << "ERROR: SHADER LINKING\n" << _infoLog << std::endl;
			}

			// The attached Shaders can be cleaned up once they are part of the overall Program
			glDetachShader(_programId, vertexShader);
			glDetachShader(_programId, fragmentShader);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			return (_success != GL_FALSE);
		}

		void AddAttribute(int index, const std::string name) {
			glBindAttribLocation(_programId, index, name.c_str());
			_uniforms.insert(std::make_pair(name, index));
		}

		void AddUniform(const std::string& name) {
			const int location = glGetUniformLocation(_programId, name.c_str());
			if (location != -1)
				_uniforms.insert(std::make_pair(name, location));
		}

	private:
		unsigned int CreateShader(GLenum type, const char* shader_source) {
			// Create the a new Shader (VERTEX or FRAGMENT)
			unsigned int shader = glCreateShader(type);
			glShaderSource(shader, 1, &shader_source, nullptr);

			// Compile the shader and check for errors
			glCompileShader(shader);
			glGetShaderiv(shader, GL_COMPILE_STATUS, &_success);
			if (_success == GL_FALSE) {
				glGetShaderInfoLog(shader, 1024, NULL, _infoLog);
				std::cout << "ERROR: SHADER COMPILATION\n" << _infoLog << std::endl;
			}

			return shader;
		}

	private:
		unsigned int _programId;
		std::map<std::string, int> _attributes;
		std::map<std::string, int> _uniforms;
		// Error logging
		int _success;
		char _infoLog[1024];
	};

}

#endif
