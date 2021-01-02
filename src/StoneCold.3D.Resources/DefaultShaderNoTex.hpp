
#ifndef STONECOLD_DEFAULTSHADERNOTEX_H
#define STONECOLD_DEFAULTSHADERNOTEX_H

#include "Shader.hpp"

namespace StoneCold::Resources {

	class DefaultShaderNoTex : public Shader {
	private:
		const std::string _vertexShader =
			"#version 410 core"
			"\n"
			"layout(location = 0) in vec3 position;"
			"layout(location = 1) in vec3 normal;"
			"layout(location = 2) in vec3 tangent;"
			"layout(location = 3) in vec2 texCoords;"
			"\n"
			"uniform mat4 u_transform;"
			"uniform mat4 u_projection;"
			"uniform mat4 u_view;"
			"\n"
			"void main() {"
			"	vec4 objPosition = u_transform * vec4(position, 1.0);"
			"	gl_Position = u_projection * u_view * objPosition;"
			"}";

		const std::string _fragmentShader =
			"#version 410 core"
			"\n"
			"out vec4 color;"
			"\n"
			"void main() {"
			"	color = vec4(0.1f, 0.7f, 0.5f, 1.0f);"
			"}";

	public:
		DefaultShaderNoTex() : Shader() {
			auto result = CreateShaderProgramm(_vertexShader, _fragmentShader);
			if (result) {
				// Set the Attributes
				AddAttribute(0, "position");
				AddAttribute(1, "normal");
				AddAttribute(2, "tangent");
				AddAttribute(3, "texCoords");
				// Set the Uniforms
				AddUniform("u_transform");
				AddUniform("u_projection");
				AddUniform("u_view");
				//AddUniform("u_objectColor");
			}
		}

	};

}

#endif
