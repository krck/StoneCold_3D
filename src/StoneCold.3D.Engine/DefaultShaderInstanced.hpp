
#ifndef STONECOLD_DEFAULTSHADERINSTANCED_H
#define STONECOLD_DEFAULTSHADERINSTANCED_H

#include "Shader.hpp"

namespace StoneCold::Engine {

    class DefaultShaderInstanced : public Shader {
    private:
        const std::string _vertexShader =
            "#version 410 core"
            "\n"
            "layout(location = 0) in vec3 position;"
            "layout(location = 1) in vec3 normal;"
            "layout(location = 2) in vec3 tangent;"
            "layout(location = 3) in vec2 texCoords;"
            "layout(location = 4) in mat4 instanceMatrix;"
            "\n"
            "uniform mat4 u_projection;"
            "uniform mat4 u_view;"
            "\n"
            "out vec2 textureCoords;"
            "\n"
            "void main() {"
            "    gl_Position = u_projection * u_view * instanceMatrix * vec4(position, 1.0);"
            "    textureCoords = texCoords;"
            "}";

        const std::string _fragmentShader =
            "#version 410 core"
            "\n"
            "in vec2 textureCoords;"
            "\n"
            "uniform vec3 u_objectColor;"
            "uniform sampler2D u_textureDiffuse;"
            "\n"
            "out vec4 color;"
            "\n"
            "void main() {"
            "    vec4 textureColor = texture(u_textureDiffuse, textureCoords);"
            "    color = textureColor;"
            "}";

    public:
        DefaultShaderInstanced() : Shader() {
            auto result = CreateShaderProgramm(_vertexShader, _fragmentShader);
            if (result) {
                glUseProgram(_programId);
                // Set the Attributes
                AddAttribute(0, "position");
                AddAttribute(1, "normal");
                AddAttribute(2, "tangent");
                AddAttribute(3, "texCoords");
                AddAttribute(4, "instanceMatrix");
                // Set Textures
                AddUniform("u_textureDiffuse");
                // Set the Uniforms
                AddUniform("u_projection");
                AddUniform("u_view");
                //AddUniform("u_objectColor");
                glUseProgram(0);
            }
        }
    };

}

#endif
