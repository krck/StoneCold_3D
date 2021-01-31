
#ifndef STONECOLD_SHADERDEFAULT_H
#define STONECOLD_SHADERDEFAULT_H

#include "Shader.hpp"

namespace StoneCold::Engine {

	class ShaderDefault : public Shader {
	private:
         const std::string _vertexShader =
         "#version 410 core"
         "\n"
         // Input vertex data, different for all executions of this shader
         "layout(location = 0) in vec3 vertexPosition_modelspace;"
         "layout(location = 1) in vec3 vertexNormal_modelspace;"
         "layout(location = 2) in vec3 vertexTangent_modelspace;"
         "layout(location = 3) in vec3 vertexBitangent_modelspace;"
         "layout(location = 4) in vec2 vertexUV;"
         "\n"
         // Values that stay constant for the whole mesh.
         "uniform mat4 u_ViewMatrix;"
         "uniform mat4 u_TransformMatrix;"
         "uniform mat4 u_ProjectionMatrix;"
         "uniform vec3 u_LightPosition_worldspace;"
         "\n"
         // Output data ; will be interpolated for each fragment.
         "out vec2 UV;"
         "out vec3 Position_worldspace;"
         "out vec3 EyeDirection_cameraspace;"
         "out vec3 LightDirection_cameraspace;"
         "out vec3 LightDirection_tangentspace;"
         "out vec3 EyeDirection_tangentspace;"
         "\n"
         "void main() {"
             // Output position of the vertex, in clip space : MVP * position
         "   gl_Position =  u_ProjectionMatrix * u_ViewMatrix * u_TransformMatrix * vec4(vertexPosition_modelspace,1);"
         ""
             // Position of the vertex, in worldspace : ModelMatrix * position
         "   Position_worldspace = (u_TransformMatrix * vec4(vertexPosition_modelspace,1)).xyz;"
         ""
             // Vector that goes from the vertex to the camera, in camera space.
             // In camera space, the camera is at the origin (0,0,0).
         "   vec3 vertexPosition_cameraspace = (u_ViewMatrix * u_TransformMatrix * vec4(vertexPosition_modelspace,1)).xyz;"
         "   EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;"
         ""
             // Vector that goes from the vertex to the light, in camera space. ModelMatrix is ommited because it's identity.
         "   vec3 LightPosition_cameraspace = (u_ViewMatrix * vec4(u_LightPosition_worldspace,1)).xyz;"
         "   LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;"
         ""
             // UV of the vertex. No special space for this one.
         "   UV = vertexUV;"
         ""
             // model to camera = ModelView
         "   mat3 MV3x3 = mat3(u_ViewMatrix * u_TransformMatrix);"
         "   vec3 vertexTangent_cameraspace = MV3x3 * vertexTangent_modelspace;"
         "   vec3 vertexBitangent_cameraspace = MV3x3 * vertexBitangent_modelspace;"
         "   vec3 vertexNormal_cameraspace = MV3x3 * vertexNormal_modelspace;"
         ""
             // You can use dot products instead of building this matrix and transposing it. See References for details.
         "   mat3 TBN = transpose(mat3("
         "       vertexTangent_cameraspace,"
         "       vertexBitangent_cameraspace,"
         "       vertexNormal_cameraspace"
         "   ));"
         ""
         "   LightDirection_tangentspace = TBN * LightDirection_cameraspace;"
         "   EyeDirection_tangentspace =  TBN * EyeDirection_cameraspace;"
         "}";

         const std::string _fragmentShader =
         "#version 410 core"
         "\n"
         // Interpolated values from the vertex shaders
         "in vec2 UV;"
         "in vec3 Position_worldspace;"
         "in vec3 EyeDirection_cameraspace;"
         "in vec3 LightDirection_cameraspace;"
         "in vec3 LightDirection_tangentspace;"
         "in vec3 EyeDirection_tangentspace;"
         "\n"
         // Values that stay constant for the whole mesh.
         "uniform sampler2D u_DiffuseTextureSampler;"
         "uniform sampler2D u_NormalTextureSampler;"
        // "//uniform sampler2D u_SpecularTextureSampler;"
         "uniform vec3 u_LightPosition_worldspace;"
         "\n"
         // Ouput data
         "out vec3 color;"
         "\n"
         "void main() {"
             // Light emission properties
             // You probably want to put them as uniforms
         "   vec3 LightColor = vec3(1,0.8,0.6);"
         "   float LightPower = 100.0;"
         ""
             // Material properties
         "   vec3 MaterialDiffuseColor = texture( u_DiffuseTextureSampler, UV ).rgb;"
         "   vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;"
        // "   //vec3 MaterialSpecularColor = texture( SpecularTextureSampler, UV ).rgb * 0.3;"
         ""
             // Local normal, in tangent space. V tex coordinate is inverted because normal map is in TGA (not in DDS) for better quality
         "   vec3 TextureNormal_tangentspace = normalize(texture( u_NormalTextureSampler, vec2(UV.x,-UV.y) ).rgb*2.0 - 1.0);"
         ""
             // Distance to the light
         "   float distance = length( u_LightPosition_worldspace - Position_worldspace );"
         ""
             // Normal of the computed fragment, in camera space
         "   vec3 n = TextureNormal_tangentspace;"
             // Direction of the light (from the fragment to the light)
         "   vec3 l = normalize(LightDirection_tangentspace);"
             // Cosine of the angle between the normal and the light direction,
             // clamped above 0
             //  - light is at the vertical of the triangle -> 1
             //  - light is perpendicular to the triangle -> 0
             //  - light is behind the triangle -> 0
         "   float cosTheta = clamp( dot( n,l ), 0,1 );"
         ""
             // Eye vector (towards the camera)
         "   vec3 E = normalize(EyeDirection_tangentspace);"
             // Direction in which the triangle reflects the light
         "   vec3 R = reflect(-l,n);"
             // Cosine of the angle between the Eye vector and the Reflect vector,
             // clamped to 0
             //  - Looking into the reflection -> 1
             //  - Looking elsewhere -> < 1
         "   float cosAlpha = clamp( dot( E,R ), 0,1 );"
         ""
             // Ambient : simulates indirect lighting + Diffuse : "color" of the object + Specular : reflective highlight, like a mirror
        // "   color = MaterialAmbientColor + ((MaterialDiffuseColor * LightColor * LightPower * cosTheta) / distance);"
        "   color = vec3(1.0, 1.0, 0.0);"
         "}";

	public:
		ShaderDefault() : Shader() {
			auto result = CreateShaderProgramm(_vertexShader, _fragmentShader);
			if (result) {
                glUseProgram(_programId);
                // Set the Attributes
                AddAttribute(0, "vertexPosition_modelspace");
                AddAttribute(1, "vertexNormal_modelspace");
                AddAttribute(2, "vertexTangent_modelspace");
                AddAttribute(3, "vertexBitangent_modelspace");
                AddAttribute(4, "vertexUV");
                // Set Textures
                AddUniform("u_DiffuseTextureSampler");
                AddUniform("u_NormalTextureSampler");
                // Set the Uniforms
                AddUniform("u_ViewMatrix");
                AddUniform("u_TransformMatrix");
                AddUniform("u_ProjectionMatrix");
                AddUniform("u_LightPosition_worldspace");
                glUseProgram(0);
			}
		}
	};

}

#endif
