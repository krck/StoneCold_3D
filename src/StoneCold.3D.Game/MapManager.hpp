
#ifndef STONECOLD_MAPMANAGER_H
#define STONECOLD_MAPMANAGER_H

#include <iostream>
#include <iomanip>
#include <random>
#include <vector>
#include <algorithm>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Types.hpp"
#include "Enums.hpp"

namespace StoneCold::Game {

	using namespace StoneCold::Base;

	//
	// Procedural Map Generator
	//
	class MapManager {
	private:
		struct Walker {
			glm::ivec2 dir = glm::ivec2();
			glm::ivec2 pos = glm::ivec2();
		};

	public:
		MapManager() { }
		MapManager(const MapManager&) = delete;
		MapManager& operator=(const MapManager&) = delete;

		std::vector<glm::vec3> GenerateMap(LevelType type, const glm::ivec2& mapSize);
		//inline std::pair<glm::ivec2, glm::ivec2> GetStartEndPositions() { return _mapStartEndPositions; }

		~MapManager() = default;

	private:
		//
		// Using the Drunkard-Walk procedural generation algorithm (The simplest dungeon map generator)
		// This Code is based on a Tutorial by William Beaty (https://www.youtube.com/watch?v=I74I_MhZIK8)
		// (... which in turn was based on the game Nuclear Throne)
		//
		void CreateMap_DrunkardWalk(const glm::ivec2& mapSize, std::vector<std::vector<MapTileTypes>>& tiles, float chanceWalkerChangeDir, float chanceWalkerUpdate);

		//
		// Removes one element form a vector at the specified index
		//
		template <typename T>
		void remove_at(std::vector<T>& v, typename std::vector<T>::size_type i) {
			std::swap(v[i], v.back());
			v.pop_back();
		}

	private:
		//std::pair<glm::ivec2, glm::ivec2> _mapStartEndPositions;
		//std::vector<std::vector<MapTileTypes>> _grid;
		//std::vector<Walker> _walkers;
		//glm::ivec2 _mapSize;

		//// Const map generation settings
		//const int _maxWalkers = 10;
		//const float _percentToFill = 0.35f;
	};

}

#endif
