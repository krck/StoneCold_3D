
#include "MapManager.hpp"

using namespace StoneCold;
using namespace StoneCold::Game;


std::vector<glm::vec3> MapManager::GenerateMap(LevelType type, const glm::ivec2& mapSize) {
	// Set _grid size and create a _grid with empty spaces and reset the _walkers
	//_mapSize = size;
	//_grid = 
	//_walkers = std::vector<Walker>();

	//CreateFloor(randomDirValue, randomUpdateValue);
	//CreateWalls();
	//SetFinalMapTiles();

	// Create a map grid with default tiles
	auto tiles = std::vector<std::vector<MapTileTypes>>(mapSize.x, std::vector<MapTileTypes>(mapSize.y, MapTileTypes::Block));
	switch (type)
	{
	case StoneCold::Base::LevelType::Grassland:		CreateMap_DrunkardWalk(mapSize, tiles, 0.95f, 0.2f);	break; // DrunkardWalk map "FLAT" style
	case StoneCold::Base::LevelType::Desert:		CreateMap_DrunkardWalk(mapSize, tiles, 0.95f, 0.2f);	break; // DrunkardWalk map "FLAT" style
	case StoneCold::Base::LevelType::Arctic:		CreateMap_DrunkardWalk(mapSize, tiles, 0.75f, 0.1f);	break; // DrunkardWalk map "MIXED" style
	case StoneCold::Base::LevelType::Highlands:		CreateMap_DrunkardWalk(mapSize, tiles, 0.75f, 0.1f);	break; // DrunkardWalk map "MIXED" style
	case StoneCold::Base::LevelType::Castle:		CreateMap_DrunkardWalk(mapSize, tiles, 0.6f, 0.01f);	break; // DrunkardWalk map "ROCKY" style
	default: break;
	}

	// Create the final map
	auto map = std::vector<glm::vec3>();
	map.reserve(((uint64)mapSize.x * mapSize.y) * 2);
	// Add the "Floor". This is simpy the whole map size filled with solid blocks at Z = 0
	// Add the "Map Layout". This is a layer at Z = 1 filled with the blocks from the procedurally generated map
	for (float x = 0; x < mapSize.x; x++) {
		for (float y = 0; y < mapSize.y; y++) {
			if (tiles[x][y] == MapTileTypes::Block)
				map.push_back(glm::vec3(x, y, 1.0f));		// Map
			else
				map.push_back(glm::vec3(x, y, 0.0f));		// Floor
		}
	}

	return map;
}


void MapManager::CreateMap_DrunkardWalk(const glm::ivec2& mapSize, std::vector<std::vector<MapTileTypes>>& tiles, float chanceWalkerChangeDir, float chanceWalkerUpdate) {
	const int maxWalkers = 10;
	const float percentToFill = 0.35f;
	const auto mapCenter = glm::ivec2((mapSize.x / 2), (mapSize.y / 2));

	// Setup the random directions to pick from
	const std::vector<glm::ivec2> directions = {
		glm::ivec2(0, -1),	// down
		glm::ivec2(-1, 0),	// left
		glm::ivec2(0, 1),	// up
		glm::ivec2(1, 0)	// right 
	};

	// Create and add the first walker (spawn in the center)
	auto newWalker = Walker();
	auto walkers = std::vector<Walker>(maxWalkers);
	newWalker.dir = directions[(((uint64)rand() % 4) + 1) - 1];
	newWalker.pos = mapCenter;
	walkers.push_back(newWalker);

	float random = 0.f;
	float blockCount = 0.f;
	for (int iterations = 0; iterations < 100000; iterations++) {
		// Random chance: Destroy a Walker (Only if its not the only one, and at a low chance)
		random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		if (random < chanceWalkerUpdate && walkers.size() > 1) {
			remove_at(walkers, (rand() % ((walkers.size() - 1) + 0)));
		}

		// Move all walkers
		for (auto& walker : walkers) {
			// Random chance: Walker picks a new direction
			random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			if (random < chanceWalkerChangeDir)
				walker.dir = directions[(((uint64)rand() % 4) + 1) - 1];

			// Update the position, based on the direction
			walker.pos.x += walker.dir.x;
			walker.pos.y += walker.dir.y;
			// Avoid the boarder of the tiles
			// Clamp x,y to leave space to the boarder (7 Tiles) and if it hits that threshold: Reset to the center of the map
			walker.pos.x = (std::clamp<uint32>(walker.pos.x, 7, mapSize.x - 8) != walker.pos.x ? mapCenter.x : walker.pos.x);
			walker.pos.y = (std::clamp<uint32>(walker.pos.y, 7, mapSize.y - 8) != walker.pos.y ? mapCenter.y : walker.pos.y);

			// Create a Floor at the position of every walker, if there is non already
			if (tiles[walker.pos.x][walker.pos.y] != MapTileTypes::Empty) {
				tiles[walker.pos.x][walker.pos.y] = MapTileTypes::Empty;
				blockCount++;
			}
		}

		// Random chance: Spawn a new Walker (Only if # of walkers < max, and at a low chance)
		random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		if (random < chanceWalkerUpdate && walkers.size() < maxWalkers) {
			auto newWalker = Walker();
			newWalker.dir = directions[(((uint64)rand() % 4) + 1) - 1];
			newWalker.pos = walkers.front().pos;
			walkers.push_back(newWalker);
		}

		// Calculate the Percentage of tiles that are "Floor" and exit if enough is filled
		if ((blockCount / (float)(mapSize.x * mapSize.y)) > percentToFill)
			break;
	}
}

