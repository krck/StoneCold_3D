
#ifndef STONECOLD_SHADERMAP_H
#define STONECOLD_SHADERMAP_H

#include "Shader.hpp"

namespace StoneCold::Engine {

    class ShaderMap : public Shader {
    private:
         const std::string _vertexShader =
         "#version 410 core"
         "\n"
         // Input vertex data, different for all executions of this shader
         "layout(location = 0) in vec3 vertexPosition_modelspace;"
         "layout(location = 1) in vec2 vertexUV;"
         "layout(location = 2) in vec3 diffuseLight;"
         "\n"
         // Values that stay constant for the whole mesh.
         "uniform mat4 u_ViewMatrix;"
         "uniform mat4 u_TransformMatrix;"
         "uniform mat4 u_ProjectionMatrix;"
         "\n"
         // Output data ; will be interpolated for each fragment.
         "out vec2 UV;"
         "out vec3 Diffuse;"
         "\n"
         "void main() {"
             // Output position of the vertex, in clip space : MVP * position
         "   gl_Position =  u_ProjectionMatrix * u_ViewMatrix * u_TransformMatrix * vec4(vertexPosition_modelspace,1);"
             // UV of the vertex. No special space for this one.
         "   UV = vertexUV;"
         "   Diffuse = diffuseLight;"
         "}";

         const std::string _fragmentShader =
         "#version 410 core"
         "\n"
         // Interpolated values from the vertex shaders
         "in vec2 UV;"
         "in vec3 Diffuse;"
         "uniform sampler2D u_DiffuseTextureSampler;"
         "\n"
         // Ouput data
         "out vec3 color;"
         "\n"
         "void main() {"
         "   color = Diffuse + texture(u_DiffuseTextureSampler, UV).rgb;"
         "}";

    public:
        ShaderMap() : Shader() {
            auto result = CreateShaderProgramm(_vertexShader, _fragmentShader);
            if (result) {
                glUseProgram(_programId);
                // Set the Attributes
                AddAttribute(0, "vertexPosition_modelspace");
                AddAttribute(1, "vertexUV");
                AddAttribute(2, "diffuseLight");
                // Set Textures
                AddUniform("u_DiffuseTextureSampler");
                // Set the Uniforms
                AddUniform("u_ViewMatrix");
                AddUniform("u_TransformMatrix");
                AddUniform("u_ProjectionMatrix");
                glUseProgram(0);
            }
        }
    };

}

#endif
