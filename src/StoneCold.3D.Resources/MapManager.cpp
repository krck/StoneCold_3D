
#include "MapManager.hpp"

using namespace StoneCold::Resources;


std::vector<ChunkMesh> MapManager::GenerateMap(const glm::ivec2& mapSize, const PerlinPreset& mapPreset, const glm::vec3& lightColor) {
    // Check if Map can be Chunk-ified without leaving any remains (and stop if not)
    if(mapSize.x % CHUNK_SIZE != 0 || mapSize.y % CHUNK_SIZE != 0)
        return std::vector<ChunkMesh>();
    
    // ------------------------------------------------------------
    // 1. Generate a height Map based on the Perlin Noise algorithm
    // ------------------------------------------------------------
    auto heightMap = std::vector<std::vector<float>>(mapSize.x, std::vector<float>(mapSize.y, 0.0f));
    GeneratePerlinNoiseMap(mapSize, mapPreset, heightMap);
    
    // --------------------------
    // 2. Generate the Map Chunks
    // --------------------------
    // By calculating the corner positions (Top left, top right, bottom left, bottom right) of each Chunk as a square area
    //  0,0 --------- 0,8       0,0 - 0,3 0,4 - 0,7
    //  |               |       |       | |       |
    //  |    Map 8x8    |       3,0 - 3,3 3,4 - 3,7
    //  |   Chunk 4x4   |       4,0 - 4,3 4,4 - 4,7
    //  |               |       |       | |       |
    //  8,0 --------- 8,8       7,0 - 7,3 7,4 - 7,7
    // These square areas will later be used to extract parts of the full Map mesh into the specific Chunks
    ChunkArea2D chunk;
    std::vector<ChunkArea2D> chunkAreas2D;
    chunkAreas2D.reserve((mapSize.x * mapSize.y) / (CHUNK_SIZE * CHUNK_SIZE));
    for (uint16 x = 0; x < (mapSize.x / CHUNK_SIZE); x++) {
        for (uint16 y = 0; y < (mapSize.y / CHUNK_SIZE); y++) {
            chunk.xMin = (x * CHUNK_SIZE);
            chunk.xMax = (x * CHUNK_SIZE) + CHUNK_SIZE - 1.0f;
            chunk.yMin = (y * CHUNK_SIZE);
            chunk.yMax = (y * CHUNK_SIZE) + CHUNK_SIZE - 1.0f;
            chunkAreas2D.push_back(chunk);
        }
    }
    
    // ---------------------------------------------------------------------
    // 3. Create the full Voxel Map as one big calculated mesh of cube faces
    // ---------------------------------------------------------------------
    auto fullMapMesh = std::vector<CubeFaceMesh>();
    fullMapMesh.reserve((mapSize.x * mapSize.y) * 5);
    float z = 0.0f, zRight = 0.0f, zLeft = 0.0f, zFront = 0.0f, zBack = 0.0f;
    // Loop each XY position of the HeightMap and based on the generated Z heights and add faces
    for (float x = 0.0f; x < mapSize.x; x++) {
        for (float y = 0.0f; y < mapSize.y; y++) {
            z = heightMap[x][y];
            // Check if the space left/right/in-front/behind the cube is empty and add a specific face to the mesh
            // (Loop is required in case multiple spaces are empty and need to be filled, stacked on top of each other)
            if (x + 1.0f < mapSize.x && heightMap[x + 1.0f][y] < z) {
                zRight = heightMap[x + 1.0f][y];
                for(float diff = 0.0f; diff < (z - zRight); diff++)
                    GetCubeFaceMesh(CubeFace::Right, x, (diff + zRight + 1.0f), y, fullMapMesh);
            }
            if (x > 0.0f && heightMap[x - 1.0f][y] < z) {
                zLeft = heightMap[x - 1.0f][y];
                for(float diff = 0.0f; diff < (z - zLeft); diff++)
                    GetCubeFaceMesh(CubeFace::Left, x, (diff + zLeft + 1.0f), y, fullMapMesh);
            }
            if (y + 1.0f < mapSize.y && heightMap[x][y + 1.0f] < z) {
                zFront = heightMap[x][y + 1.0f];
                for(float diff = 0.0f; diff < (z - zFront); diff++)
                    GetCubeFaceMesh(CubeFace::Front, x, (diff + zFront + 1.0f), y, fullMapMesh);
            }
            if (y > 0.0f && heightMap[x][y - 1.0f] < z) {
                zBack = heightMap[x][y - 1.0f];
                for(float diff = 0.0f; diff < (z - zBack); diff++)
                    GetCubeFaceMesh(CubeFace::Back, x, (diff + zBack + 1.0f), y, fullMapMesh);
            }
            // Always add a face to the top of the "Cube" (and never on the bottom)
            GetCubeFaceMesh(CubeFace::Top, x, z, y, fullMapMesh);
        }
    }
    
    // ----------------------------------------------------------------------------------------
    // 4. Putting it all together: Create Chunk-Meshes based on the Chunks and the big Map-Mesh
    // ----------------------------------------------------------------------------------------
    std::vector<ChunkMesh> mapChunkMeshes;
    mapChunkMeshes.reserve((mapSize.x * mapSize.y) / (CHUNK_SIZE * CHUNK_SIZE));
    for(const auto& chunk : chunkAreas2D) {
        // 4.1 Exctract the Chunk Mesh from the full Map Mesh
        std::vector<Vertex> chunkMesh;
        chunkMesh.reserve((CHUNK_SIZE * CHUNK_SIZE) * 6);
        for (const auto& cfm : fullMapMesh) {
            // Get a flat list of Vertecies (2 Triangles / 6 Vertecies) based on all CubeFaceMeshes, that fall within the bounds of the current Chunk
            if(cfm.mapPosition.x >= chunk.xMin && cfm.mapPosition.x <= chunk.xMax && cfm.mapPosition.z >= chunk.yMin && cfm.mapPosition.z <= chunk.yMax) {
                chunkMesh.push_back(Vertex { cfm.vertices[0], cfm.normal, glm::vec3(), glm::vec3(), cfm.texCoords[0] });
                chunkMesh.push_back(Vertex { cfm.vertices[1], cfm.normal, glm::vec3(), glm::vec3(), cfm.texCoords[1] });
                chunkMesh.push_back(Vertex { cfm.vertices[2], cfm.normal, glm::vec3(), glm::vec3(), cfm.texCoords[2] });
                chunkMesh.push_back(Vertex { cfm.vertices[3], cfm.normal, glm::vec3(), glm::vec3(), cfm.texCoords[3] });
                chunkMesh.push_back(Vertex { cfm.vertices[4], cfm.normal, glm::vec3(), glm::vec3(), cfm.texCoords[4] });
                chunkMesh.push_back(Vertex { cfm.vertices[5], cfm.normal, glm::vec3(), glm::vec3(), cfm.texCoords[5] });
            }
        }
        
        // 4.2 Calculate the indices for the Voxel Map-Chunk mesh
        int index = 0;
        std::vector<uint32> indices;
        std::set<std::pair<Vertex, uint32>, VertexCompareClass> vertexIndexPairs;
        for (uint64 i = 0; i < chunkMesh.size(); i++) {
            // Check if a Vertex-Index pair already exists based on the VertexCompareClass (0 as param)
            // (If none exists then create a new Vertex-Index pair, otherwise just add the index)
            auto iter = vertexIndexPairs.find(std::make_pair(chunkMesh[i], 0));
            if (iter == vertexIndexPairs.end()) {
                vertexIndexPairs.insert(std::make_pair(chunkMesh[i], index));
                indices.push_back(index++);
            } else {
                indices.push_back(iter->second);
            }
        }
        
        // 4.3 Finally create the (sorted) list of all unique MapVertecies with "baked-in" diffuse light
        std::vector<MapVertex> chunkVertices;
        chunkVertices.resize(vertexIndexPairs.size());
        for (const auto& vertexIndex : vertexIndexPairs) {
            // Calculate a very basic Diffuse Light right here and only once, to save calculation time in the Shader
            // (The "Light Source" is hardcoded to always be 40 units above, 10 units right and 10 units in front of the current Cube)
            glm::vec3 lightPos = glm::vec3(vertexIndex.first.Position.x + 10.0f, 40.0f, vertexIndex.first.Position.z - 10.0f);
            glm::vec3 lightDir = glm::normalize(lightPos - vertexIndex.first.Position);
            float distance = glm::length(lightPos - vertexIndex.first.Position);
            float cosTheta = glm::clamp(glm::dot(vertexIndex.first.Normal, lightDir), 0.0f, 1.0f);
            glm::vec3 diffuseLight = (lightColor * 150.0f * cosTheta / (distance * distance));
            // Based on the default Vertex data and the calculated light create a reduced MapVertex and store it as part of the Chunk
            chunkVertices[vertexIndex.second] = MapVertex { vertexIndex.first.Position, vertexIndex.first.TextureCoords, diffuseLight };
        }
        
        // Add the sorted and unique Vertices, the Indices and some info about the Chunk to the Map
        mapChunkMeshes.push_back(ChunkMesh { chunkVertices, indices, chunk });
    }
    
    return mapChunkMeshes;
}


void MapManager::GeneratePerlinNoiseMap(const glm::ivec2& mapSize, const PerlinPreset& preset, std::vector<std::vector<float>>& out_heights) const {
    const float scaleX = (1.0f / (mapSize.x - 1.0f));
    const float scaleY = (1.0f / (mapSize.y - 1.0f));
    const float G = 2.0f * exp(-preset.AmplitudeDampener);
    
    for (uint16 w = 0; w < mapSize.x; w++) {
        for (uint16 h = 0; h < mapSize.y; h++) {
            const float xs = w * scaleX;
            const float ys = h * scaleY;
            float amplitude = 1.0f;
            float frequency = 1.0f;
            float normalization = 0.0f;
            float total = 0.0f;
            
            for (uint16 i = 0; i < preset.Octaves; ++i) {
                const float noiseValue = glm::perlin(glm::vec2((xs * frequency), (ys * frequency))) * 0.5 + 0.5;
                total += noiseValue * amplitude;
                frequency *= preset.FrequencyMulti;
                normalization += amplitude;
                amplitude *= G;
            }
            
            out_heights[w][h] = floor(pow((total / normalization), preset.FlattenFactor) * preset.HeightMulti);
        }
    }
}


void MapManager::GetCubeFaceMesh(CubeFace face, float world_X, float world_Y, float world_Z, std::vector<CubeFaceMesh>& out_fullMapMesh) const {
    CubeFaceMesh tmpFace;
    tmpFace.mapPosition = glm::vec3(world_X, world_Y, world_Z);
    
    // Cube (Face) Vertices and Normal can be set based on the passed CubeFace-Type
    // Set some default values in case of CubeFace::Bottom. This should never occur
    if (face == CubeFace::Top) {
        tmpFace.normal = glm::vec3(0.0f, 1.0f, 0.0f);
        tmpFace.vertices = StoneCold::Resources::StaticResources::MapCubeFace_Top;
    }
    else if (face == CubeFace::Front) {
        tmpFace.normal = glm::vec3(0.0f, 0.0f, 1.0f);
        tmpFace.vertices = StoneCold::Resources::StaticResources::MapCubeFace_Front;
    }
    else if (face == CubeFace::Back) {
        tmpFace.normal = glm::vec3(0.0f, 0.0f, -1.0f);
        tmpFace.vertices = StoneCold::Resources::StaticResources::MapCubeFace_Back;
    }
    else if (face == CubeFace::Right) {
        tmpFace.normal = glm::vec3(1.0f, 0.0f, 0.0f);
        tmpFace.vertices = StoneCold::Resources::StaticResources::MapCubeFace_Right;
    }
    else if (face == CubeFace::Left) {
        tmpFace.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
        tmpFace.vertices = StoneCold::Resources::StaticResources::MapCubeFace_Left;
    }
    else {
        tmpFace.normal = glm::vec3(0.0f, -1.0f, 0.0f);
        tmpFace.vertices = std::vector<glm::vec3>(6);
    }
    
    // Translate the Cube (Face) Model into World-Space within the Chunk
    for(uint16 i = 0; i < tmpFace.vertices.size(); i++) {
        tmpFace.vertices[i].x = tmpFace.vertices[i].x + world_X;
        tmpFace.vertices[i].y = tmpFace.vertices[i].y + world_Y;
        tmpFace.vertices[i].z = tmpFace.vertices[i].z + world_Z;
    }
    
    // Set the Texture based on the passed height in World-Space
    if(world_Y <= 2.0f)         tmpFace.texCoords = StoneCold::Resources::StaticResources::MapTexture_GrassLight;
    else if(world_Y <= 10.0f)   tmpFace.texCoords = StoneCold::Resources::StaticResources::MapTexture_Grass;
    else if(world_Y <= 20.0f)   tmpFace.texCoords = StoneCold::Resources::StaticResources::MapTexture_Rock;
    else                        tmpFace.texCoords = StoneCold::Resources::StaticResources::MapTexture_Snow;
    
    // Add these Cube face Vertices/Normal/TexCoords to the Chunks overall mesh data
    out_fullMapMesh.push_back(tmpFace);
}
