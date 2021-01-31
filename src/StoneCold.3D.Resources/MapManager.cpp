
#include "MapManager.hpp"

#include <iostream>

using namespace StoneCold::Game;


std::vector<MapMesh> MapManager::GenerateMap(LevelType type, const glm::ivec2& mapSize, float samples, float heightRange) {
    // Generate a height Map based on the Perlin Noise algorithm
    // (perlin noise calculates a Z hight for each 2D XY Position)
    auto heightMap = std::vector<std::vector<float>>(mapSize.x, std::vector<float>(mapSize.y, 0.0f));
    GeneratePerlinNoiseMap(mapSize, heightMap, PerlinPreset());
    
    // Check if Map can be Chunk-ified without leaving any remains
    if(mapSize.x % CHUNK_SIZE != 0 || mapSize.y % CHUNK_SIZE != 0)
        return std::vector<MapMesh>();
    
    // Generate Map Chunks
    // By calculating the corner positions (Top left, top right, bottom left, bottom right) of each Chunk as a square area
    //  0,0 ------------- 0,8       0,0 --- 0,3 0,4 --- 0,7
    //  |                   |       |         | |         |
    //  |   Map Size 8x8    |       |         | |         |
    //  |  Chunk Size 4x4   |       3,0 --- 3,3 3,4 --- 3,7
    //  |                   |       4,0 --- 4,3 4,4 --- 4,7
    //  |                   |       |         | |         |
    //  |                   |       |         | |         |
    //  8,0 ------------- 8,8       7,0 --- 7,3 7,4 --- 7,7
    // These square areas will later be used to extract parts of the full Map mesh into the specific Chunks
    auto chunk = ChunkArea2D();
    auto chunkAreas2D = std::vector<ChunkArea2D>();
    chunkAreas2D.reserve((mapSize.x * mapSize.y) / (CHUNK_SIZE * CHUNK_SIZE));
    for (int x = 0; x < (mapSize.x / CHUNK_SIZE); x++) {
        for (int y = 0; y < (mapSize.y / CHUNK_SIZE); y++) {
            chunk.xMin = (x * CHUNK_SIZE);
            chunk.xMax = (x * CHUNK_SIZE) + CHUNK_SIZE - 1;
            chunk.yMin = (y * CHUNK_SIZE);
            chunk.yMax = (y * CHUNK_SIZE) + CHUNK_SIZE - 1;
            chunkAreas2D.push_back(chunk);
        }
    }
    
    // Create the full Voxel Map as one big calculated mesh of cube faces
    // Loop each XY position of the HeightMap and based on the generated Z heights and add faces
    auto fullMapMesh = std::vector<MapMeshFace>();
    fullMapMesh.reserve(mapSize.x * mapSize.y);
    for (float x = 0; x < mapSize.x; x++) {
        for (float y = 0; y < mapSize.y; y++) {
            auto z = heightMap[x][y];
            // Check if the space left/right/in-front/behind the cube is empty and a specific face to the mesh
            // Loop is required in case multiple spaces are empty and need to be filled, stacked on top of each other
            if (x + 1 < mapSize.x && heightMap[x+1][y] < z) {
                auto zRight = heightMap[x+1][y];
                for(uint32 diff = 0; diff < (z - zRight); diff++)
                    AddMapVertexData(GetMapMeshFace(CubeFace::Right), x, (diff + zRight + 1), y, fullMapMesh);
            }
            if (x > 0 && heightMap[x-1][y] < z) {
                auto zLeft = heightMap[x-1][y];
                for(uint32 diff = 0; diff < (z - zLeft); diff++)
                    AddMapVertexData(GetMapMeshFace(CubeFace::Left), x, (diff + zLeft + 1), y, fullMapMesh);
            }
            if (y + 1 < mapSize.y && heightMap[x][y+1] < z) {
                auto zFront = heightMap[x][y+1];
                for(uint32 diff = 0; diff < (z - zFront); diff++)
                    AddMapVertexData(GetMapMeshFace(CubeFace::Front), x, (diff + zFront + 1), y, fullMapMesh);
            }
            if (y > 0 && heightMap[x][y-1] < z) {
                auto zBack = heightMap[x][y-1];
                for(uint32 diff = 0; diff < (z - zBack); diff++)
                    AddMapVertexData(GetMapMeshFace(CubeFace::Back), x, (diff + zBack + 1), y, fullMapMesh);
            }
            // Add always add a face to the top of the cube (and never on the bottom)
            AddMapVertexData(GetMapMeshFace(CubeFace::Top), x, z, y, fullMapMesh);
        }
    }
    
    // Global "sun" Light of this map (should be a paramter)
    glm::vec3 lightColor = glm::vec3(SC_COLOR_YELLOW.x / 255.f, SC_COLOR_YELLOW.y / 255.f, SC_COLOR_YELLOW.z / 255.f);
    
    Vertex tmpvec;
    auto mapMeshChunks = std::vector<MapMesh>();
    mapMeshChunks.reserve((mapSize.x * mapSize.y) / (CHUNK_SIZE * CHUNK_SIZE));
    for(const auto& chunk : chunkAreas2D) {
        // Exctract the Chunk Mesh from the full Map Mesh (get all Vertices)
        auto chunkMesh = std::vector<Vertex>();
        chunkMesh.reserve((CHUNK_SIZE * CHUNK_SIZE) * 6);
        for (const auto& mapVertex : fullMapMesh) {
            // Check if the cube face is within the bounds of this Map Chunk
            if(mapVertex.mapPosition.x >= chunk.xMin && mapVertex.mapPosition.x <= chunk.xMax
               && mapVertex.mapPosition.z >= chunk.yMin && mapVertex.mapPosition.z <= chunk.yMax) {
                for (uint16 i = 0; i < mapVertex.vertices.size(); i++) {
                    tmpvec.Position.x = mapVertex.vertices[i].x + mapVertex.mapPosition.x;
                    tmpvec.Position.y = mapVertex.vertices[i].y + mapVertex.mapPosition.y;
                    tmpvec.Position.z = mapVertex.vertices[i].z + mapVertex.mapPosition.z;
                    tmpvec.Normal.x = mapVertex.normal.x;
                    tmpvec.Normal.y = mapVertex.normal.y;
                    tmpvec.Normal.z = mapVertex.normal.z;
                    tmpvec.TextureCoords.x = mapVertex.texcoords[i].x;
                    tmpvec.TextureCoords.y = mapVertex.texcoords[i].y;
                    chunkMesh.push_back(tmpvec);
                }
            }
        }
        
        // Calculate the indices for the Voxel Map-Chunk mesh
        int index = 0;
        std::vector<uint32> indices;
        std::set<VertexIndexPair, VertexCompareClass> vertices;
        for (uint64 i = 0; i < chunkMesh.size(); i++) {
            // Check if a VertexIndexPair already exists based on the VertexCompareClass (0 as param)
            std::set<VertexIndexPair>::iterator it = vertices.find(std::make_pair(chunkMesh[i], 0));
            if (it == vertices.end()) {
                // If none exists then create a new VertexIndexPair
                vertices.insert(std::make_pair(chunkMesh[i], index));
                indices.push_back(index++);
            } else {
                // If one exists already just store the index
                indices.push_back(it->second);
            }
        }
        
        // Create a new sorted vector of all unique Vertices with "baked-in" diffuse light
        auto mapVerticesIndexed = std::vector<VertexMapCube>();
        mapVerticesIndexed.resize(vertices.size());
        for (std::set<VertexIndexPair>::iterator it= vertices.begin(); it != vertices.end(); it++) {
            auto vertex = it->first;
            
            glm::vec3 lightPos = glm::vec3(vertex.Position.x + 10.0f, 40.0f, vertex.Position.z - 10.0f);
            glm::vec3 lightDir = glm::normalize(lightPos - vertex.Position);
            float distance = glm::length(lightPos - vertex.Position);
            float cosTheta = glm::clamp(glm::dot(vertex.Normal, lightDir), 0.0f, 1.0f);
            glm::vec3 diffuseLight = (lightColor * 150.0f * cosTheta / (distance * distance));
            
            mapVerticesIndexed[it->second] = VertexMapCube {
                it->first.Position,
                it->first.TextureCoords,
                (diffuseLight)
            };
        }
        
        // Add the sorted and unique Vertices and their Indices of this Chunk to the Map
        mapMeshChunks.push_back(std::make_pair(mapVerticesIndexed, indices));
    }
    
    return mapMeshChunks;
}


void MapManager::GeneratePerlinNoiseMap(const glm::ivec2& mapSize, std::vector<std::vector<float>>& heights, const PerlinPreset& preset) const {
    float xFactor = 1.0f / (mapSize.x - 1);
    float yFactor = 1.0f / (mapSize.y - 1);
    
    for (uint32 w = 0; w < mapSize.x; w++) {
        for (uint32 h = 0; h < mapSize.y; h++) {
            float x = xFactor * w;
            float y = yFactor * h;
            float perlin = GetPerlin2D(x, y, preset.FREQUENT, preset.AMPLITUDE, preset.PERSISTENCE, preset.OCTAVES);
            heights[w][h] = floor(perlin * 20);
        }
    }
}


float MapManager::GetPerlin2D(const float& x, const float& y, float frequent, float amplitude, float persistance, uint32 octaves) const {
    float total = 0.0f;
    float freq = frequent;
    float amp = amplitude;
    float maxValue = 0.0f;
    
    for (uint32 i = 0; i < octaves; ++i) {
        float p = glm::perlin(glm::vec2((x * freq), (y * freq)));
        total += ((p + 1.0f) / 2.0f) * amp;
        maxValue += amp;
        amp *= persistance;
        freq *= 2;
    }
    
    return total / maxValue;
}


void MapManager::AddMapVertexData(const MapMeshFace& face, float x, float y, float z, std::vector<MapMeshFace>& mapVertecies) const {
    auto tmpFace = face;
    tmpFace.mapPosition.x = x;
    tmpFace.mapPosition.y = y;
    tmpFace.mapPosition.z = z;
    mapVertecies.push_back(tmpFace);
}


MapMeshFace MapManager::GetMapMeshFace(CubeFace face) const {
    if (face == CubeFace::Top)
        // Cube part facing upwards (Y 1.0)
        return MapMeshFace {
            glm::vec3(0.0f, 0.0f, 0.0f),
            {
                glm::vec3(1.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(1.0f, 0.0f, 1.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f)
            },
            {
                glm::vec2(1.0f, 0.0f),
                glm::vec2(0.0f, 1.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(1.0f, 0.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(0.0f, 1.0f)
            },
            glm::vec3(0.0f, 1.0f, 0.0f)
        };
    else if (face == CubeFace::Front)
        // Cube part facing forwards (Z 1.0)
        return MapMeshFace {
            glm::vec3(0.0f, 0.0f, 0.0f),
            {
                glm::vec3(1.0f, -1.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, -1.0f, 1.0f),
                glm::vec3(1.0f, -1.0f, 1.0f),
                glm::vec3(1.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f)
            },
            {
                glm::vec2(1.0f, 0.0f),
                glm::vec2(0.0f, 1.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(1.0f, 0.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(0.0f, 1.0f)
            },
            glm::vec3(0.0f, 0.0f, 1.0f)
        };
    else if (face == CubeFace::Back)
        // Cube part facing backwards (Z -1.0)
        return MapMeshFace {
            glm::vec3(0.0f, 0.0f, 0.0f),
            {
                glm::vec3(0.0f, -1.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, -1.0f, 0.0f),
                glm::vec3(0.0f, -1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f)
            },
            {
                glm::vec2(1.0f, 0.0f),
                glm::vec2(0.0f, 1.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(1.0f, 0.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(0.0f, 1.0f)
            },
            glm::vec3(0.0f, 0.0f, -1.0f)
        };
    else if (face == CubeFace::Right)
        // Cube part facing right (X 1.0)
        return MapMeshFace {
            glm::vec3(0.0f, 0.0f, 0.0f),
            {
                glm::vec3(1.0f, -1.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 1.0f),
                glm::vec3(1.0f, -1.0f, 1.0f),
                glm::vec3(1.0f, -1.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 1.0f)
            },
            {
                glm::vec2(1.0f, 0.0f),
                glm::vec2(0.0f, 1.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(1.0f, 0.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(0.0f, 1.0f)
            },
            glm::vec3(1.0f, 0.0f, 0.0f)
        };
    else if (face == CubeFace::Left)
        // Cube part facing left (X -1.0)
        return MapMeshFace {
            glm::vec3(0.0f, 0.0f, 0.0f),
            {
                glm::vec3(0.0f, -1.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, -1.0f, 0.0f),
                glm::vec3(0.0f, -1.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f)
            },
            {
                glm::vec2(1.0f, 0.0f),
                glm::vec2(0.0f, 1.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(1.0f, 0.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(0.0f, 1.0f)
            },
            glm::vec3(1.0f, 0.0f, 0.0f)
        };
    // Bottom face should never be used
    else return MapMeshFace();
}
