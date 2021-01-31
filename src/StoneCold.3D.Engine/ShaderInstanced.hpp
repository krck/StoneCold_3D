
#ifndef STONECOLD_SHADERINSTANCED_H
#define STONECOLD_SHADERINSTANCED_H

#include "Shader.hpp"

namespace StoneCold::Engine {

    class ShaderInstanced : public Shader {
    private:
        const std::string _vertexShader =
        "#version 410 core"
        "\n"
        "layout (location = 0) in vec3 aPos;"
        "layout (location = 1) in vec3 aNormal;"
        "layout (location = 2) in vec3 aTangent;"
        "layout (location = 3) in vec3 aBitangent;"
        "layout (location = 4) in vec2 aTexCoords;"
        "layout (location = 5) in mat4 instanceMatrix;"
        "\n"
        "out VS_OUT {"
        "    vec3 FragPos;"
        "    vec2 TexCoords;"
        "    vec3 TangentLightPos;"
        "    vec3 TangentViewPos;"
        "    vec3 TangentFragPos;"
        "} vs_out;"
        "\n"
        "uniform mat4 projection;"
        "uniform mat4 view;"
        "\n"
        "uniform vec3 lightPos;"
        "\n"
        "void main()"
        "{"
        "    vs_out.FragPos = vec3(instanceMatrix * vec4(aPos, 1.0));"
        "    vs_out.TexCoords = aTexCoords;"
        ""
        "   vec3 vertexPosition_cameraspace = (view * instanceMatrix * vec4(aPos,1)).xyz;"
        "   vec3 viewPos = vec3(0,0,0) - vertexPosition_cameraspace;"
        ""
        "    mat3 normalMatrix = transpose(inverse(mat3(instanceMatrix)));"
        "    vec3 T = normalize(normalMatrix * aTangent);"
        "    vec3 N = normalize(normalMatrix * aNormal);"
        "    T = normalize(T - dot(T, N) * N);"
        "    vec3 B = cross(N, T);"
        ""
        "    mat3 TBN = transpose(mat3(T, B, N));"
        "    vs_out.TangentLightPos = TBN * lightPos;"
        "    vs_out.TangentViewPos  = TBN * viewPos;"
        "    vs_out.TangentFragPos  = TBN * vs_out.FragPos;"
        ""
        "    gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);"
        "}";
        
        /*
        "#version 410 core"
        "\n"
        // Input vertex data, different for all executions of this shader
        // instanceMatrix acts as the Model-Matrix
        "layout(location = 0) in vec3 vertexPosition_modelspace;"
        "layout(location = 1) in vec3 vertexNormal_modelspace;"
        "layout(location = 2) in vec3 vertexTangent_modelspace;"
        "layout(location = 3) in vec3 vertexBitangent_modelspace;"
        "layout(location = 4) in vec2 vertexUV;"
        "layout(location = 5) in mat4 instanceMatrix;"
        "\n"
        // Values that stay constant for the whole mesh.
        "uniform mat4 u_ViewMatrix;"
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
        "   gl_Position =  u_ProjectionMatrix * u_ViewMatrix * instanceMatrix * vec4(vertexPosition_modelspace,1);"
        ""
            // Position of the vertex, in worldspace : ModelMatrix * position
        "   Position_worldspace = (instanceMatrix * vec4(vertexPosition_modelspace,1)).xyz;"
        ""
            // Vector that goes from the vertex to the camera, in camera space.
            // In camera space, the camera is at the origin (0,0,0).
        "   vec3 vertexPosition_cameraspace = (u_ViewMatrix * instanceMatrix * vec4(vertexPosition_modelspace,1)).xyz;"
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
        "   mat3 MV3x3 = mat3(u_ViewMatrix * instanceMatrix);"
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
         */
        
        
        const std::string _fragmentShader =
        "#version 410 core"
        "\n"
        "out vec4 FragColor;"
        "\n"
        "in VS_OUT {"
        "    vec3 FragPos;"
        "    vec2 TexCoords;"
        "    vec3 TangentLightPos;"
        "    vec3 TangentViewPos;"
        "    vec3 TangentFragPos;"
        "} fs_in;"
        "\n"
        "uniform sampler2D diffuseMap;"
        "uniform sampler2D normalMap;"
        "\n"
        "uniform vec3 lightPos;"
        "uniform vec3 viewPos;"
        "\n"
        "void main()"
        "{"
             // obtain normal from normal map in range [0,1]
        "    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;"
            // transform normal vector to range [-1,1]
        "    normal = normalize(normal * 2.0 - 1.0);"  // this normal is in tangent space
        "    normal = vec3(normal.x, -normal.y, normal.z);"
        ""
            // get diffuse color
        "    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;"
            // ambient
        "    vec3 ambient = 0.1 * color;"
            // diffuse
        "    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);"
        "    float diff = max(dot(lightDir, normal), 0.0);"
        "    vec3 diffuse = diff * color;"
            // specular
        ""
        "    FragColor = vec4(ambient + diffuse, 1.0);"
        "}";
        
        /*
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
        "   color = MaterialAmbientColor + ((MaterialDiffuseColor * LightColor * LightPower * cosTheta) / distance);"
       // "   //MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);"
        "}";
         */
         
    public:
        ShaderInstanced() : Shader() {
            auto result = CreateShaderProgramm(_vertexShader, _fragmentShader);
            if (result) {
                glUseProgram(_programId);
                // Set the Attributes
                AddAttribute(0, "aPos");
                AddAttribute(1, "aNormal");
                AddAttribute(2, "aTangent");
                AddAttribute(3, "aBitangent");
                AddAttribute(4, "aTexCoords");
                AddAttribute(5, "instanceMatrix");
                // Set Textures
                AddUniform("diffuseMap");
                AddUniform("normalMap");
                // Set the Uniforms
                AddUniform("view");
                AddUniform("projection");
                AddUniform("lightPos");
                glUseProgram(0);
            }
        }
    };

}

#endif
