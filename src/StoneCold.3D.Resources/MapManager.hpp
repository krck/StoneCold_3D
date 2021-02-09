
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
#include "StaticMapResources.hpp"
#include "Settings.hpp"

namespace StoneCold::Resources {

	using namespace StoneCold::Base;

	//
	// Procedural Map Generator
	//
	class MapManager {
	public:
		MapManager() { }
		MapManager(const MapManager&) = delete;
		MapManager& operator=(const MapManager&) = delete;

		std::vector<ChunkMesh> GenerateMap(const glm::ivec2& mapSize, const PerlinPreset& mapPreset, const glm::vec3& lightColor);

		~MapManager() = default;

	private:
        //
        // Perlin n shit
        //
        void GeneratePerlinNoiseMap(const glm::ivec2& mapSize, const PerlinPreset& preset, std::vector<std::vector<float>>& out_heights) const;
        void GetCubeFaceMesh(CubeFace face, float world_X, float world_Y, float world_Z, std::vector<CubeFaceMesh>& out_fullMapMesh) const;
	};

}

#endif
