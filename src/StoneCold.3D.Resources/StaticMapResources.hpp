
#ifndef STONECOLD_STATICMAPRESOURCES_H
#define STONECOLD_STATICMAPRESOURCES_H

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace StoneCold::Resources::StaticResources {
    
    // ------------------------------------------------------------------------
    // ------------------------ MAP CUBE (Mesh) FACES -------------------------
    // ------------------------------------------------------------------------
    
    // Cube side facing upwards
    // Normal:  Y 1.0
    // Enum:    CubeFace::Top
    static const std::vector<glm::vec3> MapCubeFace_Top = std::vector<glm::vec3>
    {
        glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f)
    };
    
    // Cube side facing forwards
    // Normal:  Z 1.0
    // Enum:    CubeFace::Front
    static const std::vector<glm::vec3> MapCubeFace_Front = std::vector<glm::vec3>
    {
        glm::vec3(1.0f, -1.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, -1.0f, 1.0f),
        glm::vec3(1.0f, -1.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    };
    
    // Cube side facing backwards
    // Normal:  Z -1.0
    // Enum:    CubeFace::Back
    static const std::vector<glm::vec3> MapCubeFace_Back = std::vector<glm::vec3>
    {
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f)
    };
    
    // Cube side facing right
    // Normal:  X 1.0
    // Enum:    CubeFace::Right
    static const std::vector<glm::vec3> MapCubeFace_Right = std::vector<glm::vec3>
    {
        glm::vec3(1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, -1.0f, 1.0f),
        glm::vec3(1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 1.0f)
    };
    
    // Cube side facing left
    // Normal:  X -1.0
    // Enum:    CubeFace::Left
    static const std::vector<glm::vec3> MapCubeFace_Left = std::vector<glm::vec3>
    {
        glm::vec3(0.0f, -1.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 0.0f)
    };
  
    // ------------------------------------------------------------------------
    // -------------------- MAP CUBE TEXTURES (in Atlas) ----------------------
    // ------------------------------------------------------------------------
    
    // Light Grass Texture
    // Coordinates based on its position in the Map Texture Atlas
    // Enum:    MapTextureType::GrassLight
    static const std::vector<glm::vec2> MapTexture_GrassLight = std::vector<glm::vec2>
    {
        glm::vec2(0.25f, 0.0f),
        glm::vec2(0.0f,  0.25f),
        glm::vec2(0.0f,  0.0f),
        glm::vec2(0.25f, 0.0f),
        glm::vec2(0.25f, 0.25f),
        glm::vec2(0.0f,  0.25f)
    };
    
    // Grass Texture
    // Coordinates based on its position in the Map Texture Atlas
    // Enum:    MapTextureType::Grass
    static const std::vector<glm::vec2> MapTexture_Grass = std::vector<glm::vec2>
    {
        glm::vec2(0.5f,  0.0f),
        glm::vec2(0.25f, 0.25f),
        glm::vec2(0.25f, 0.0f),
        glm::vec2(0.5f,  0.0f),
        glm::vec2(0.5f,  0.25f),
        glm::vec2(0.25f, 0.25f)
    };
    
    // Sand Texture
    // Coordinates based on its position in the Map Texture Atlas
    // Enum:    MapTextureType::Sand
    static const std::vector<glm::vec2> MapTexture_Sand = std::vector<glm::vec2>
    {
        glm::vec2(0.75f, 0.0f),
        glm::vec2(0.5f,  0.25f),
        glm::vec2(0.5f,  0.0f),
        glm::vec2(0.75f, 0.0f),
        glm::vec2(0.75f, 0.25f),
        glm::vec2(0.5f,  0.25f)
    };
    
    // Ice Texture
    // Coordinates based on its position in the Map Texture Atlas
    // Enum:    MapTextureType::Ice
    static const std::vector<glm::vec2> MapTexture_Ice = std::vector<glm::vec2>
    {
        glm::vec2(1.0f,  0.0f),
        glm::vec2(0.75f, 0.25f),
        glm::vec2(0.75f, 0.0f),
        glm::vec2(1.0f,  0.0f),
        glm::vec2(1.0f,  0.25f),
        glm::vec2(0.75f, 0.25f)
    };
    
    // Dirt Texture
    // Coordinates based on its position in the Map Texture Atlas
    // Enum:    MapTextureType::Dirt
    static const std::vector<glm::vec2> MapTexture_Dirt = std::vector<glm::vec2>
    {
        glm::vec2(0.25f, 0.25f),
        glm::vec2(0.0f,  0.5f),
        glm::vec2(0.0f,  0.25f),
        glm::vec2(0.25f, 0.25f),
        glm::vec2(0.25f, 0.5f),
        glm::vec2(0.0f,  0.5f)
    };
    
    // Rock Texture
    // Coordinates based on its position in the Map Texture Atlas
    // Enum:    MapTextureType::Rock
    static const std::vector<glm::vec2> MapTexture_Rock = std::vector<glm::vec2>
    {
        glm::vec2(0.5f,  0.25f),
        glm::vec2(0.25f, 0.5f),
        glm::vec2(0.25f, 0.25f),
        glm::vec2(0.5f,  0.25f),
        glm::vec2(0.5f,  0.5f),
        glm::vec2(0.25f, 0.5f)
    };
    
    // Light Sand Texture
    // Coordinates based on its position in the Map Texture Atlas
    // Enum:    MapTextureType::SandLight
    static const std::vector<glm::vec2> MapTexture_SandLight = std::vector<glm::vec2>
    {
        glm::vec2(0.75f, 0.25f),
        glm::vec2(0.5f,  0.5f),
        glm::vec2(0.5f,  0.25f),
        glm::vec2(0.75f, 0.25f),
        glm::vec2(0.75f, 0.5f),
        glm::vec2(0.5f,  0.5f)
    };
    
    // Snow Texture
    // Coordinates based on its position in the Map Texture Atlas
    // Enum:    MapTextureType::Snow
    static const std::vector<glm::vec2> MapTexture_Snow = std::vector<glm::vec2>
    {
        glm::vec2(1.0f,  0.25f),
        glm::vec2(0.75f, 0.5f),
        glm::vec2(0.75f, 0.25f),
        glm::vec2(1.0f,  0.25f),
        glm::vec2(1.0f,  0.5f),
        glm::vec2(0.75f, 0.5f)
    };
    
};

#endif
