
#ifndef STONECOLD_MAPMANAGER_H
#define STONECOLD_MAPMANAGER_H

#include <iostream>
#include <iomanip>
#include <random>
#include <set>
#include <vector>
#include <algorithm>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/noise.hpp>

#include "Types.hpp"
#include "Enums.hpp"
#include "HelperClasses.hpp"
#include "Settings.hpp"

namespace StoneCold::Game {

	using namespace StoneCold::Base;

	//
	// Procedural Map Generator
	//
	class MapManager {

        // High Mointains
        struct PerlinPreset {
            //// Plain
//            const float FREQUENT = 2;
//            const float AMPLITUDE = 2;
//            const float PERSISTENCE = 1;
//            const unsigned OCTAVES = 1;
            //
            //// Lowlands
//            const float FREQUENT = 3;
//            const float AMPLITUDE = 3;
//            const float PERSISTENCE = 2;
//            const unsigned OCTAVES = 2;
            //
            //// Highlands
            const float FREQUENT = 4;
            const float AMPLITUDE = 4.5;
            const float PERSISTENCE = 2.5;
            const unsigned OCTAVES = 3;
            //
            //// Mointains
//            const float FREQUENT = 10;
//            const float AMPLITUDE = 20;
//            const float PERSISTENCE = 0.5;
//            const unsigned OCTAVES = 5;

        };

        
	public:
		MapManager() { }
		MapManager(const MapManager&) = delete;
		MapManager& operator=(const MapManager&) = delete;

		std::vector<MapMesh> GenerateMap(LevelType type, const glm::ivec2& mapSize, float samples, float heightRange);

		~MapManager() = default;

	private:
        //
        // Perlin n shit
        //
        void GeneratePerlinNoiseMap(const glm::ivec2& mapSize, std::vector<std::vector<float>>& heights, const PerlinPreset& preset) const;
        float GetPerlin2D(const float& x, const float& y, float frequent, float amplitude, float persistance, uint32 octaves) const;
        
        void AddMapVertexData(const MapMeshFace& face, float x, float y, float z, std::vector<MapMeshFace>& mapVertecies) const;
        MapMeshFace GetMapMeshFace(CubeFace face) const;
        
	};

}

#endif
