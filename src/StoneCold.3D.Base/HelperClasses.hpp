
#ifndef STONECOLD_HELPERCLASSES_H
#define STONECOLD_HELPERCLASSES_H

#include "Types.hpp"
#include "StoneColdBase.hpp"

namespace StoneCold::Base {

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Tangent;
        glm::vec3 BiTangent;
		glm::vec2 TextureCoords;
	};
    struct MapVertex {
        glm::vec3 Position;
        glm::vec2 TextureCoords;
        glm::vec3 DiffuseLight;
    };
    
    struct ChunkArea2D {
        float xMax;
        float xMin;
        float yMax;
        float yMin;
    };
                
	struct Material {
		float AmbientReflectivity = 0.0f;
		float DiffuseReflectivity = 0.0f;
		float SpectralReflectivity = 0.0f;
		float ShineDamper = 10.0f;
	};
    
    struct PerlinPreset {
        uint16 Octaves = 4;             // !NO NEED TO CHANGE! Layers of Noise-Maps. Layers add more detail (rough and more "pronounced" features)
        float HeightMulti = 64.0f;      // !NO NEED TO CHANGE! Overall Map-Hight multiplier. Big hills get bigger, low valleys stay the same.
        float AmplitudeDampener = 0.0f; // Low: Bigger hills and more "pronounced" terrain - High: Low hills and overall flat terrain
        float FrequencyMulti = 0.0f;    // Low: Few, flat and far apart hills - High: Many, big and close to each other hills
        float FlattenFactor = 0.0f;     // Low: Produces maps with big hills and deep valles - High: Produces flat maps with minimal elevation
    };
    
    struct CubeFaceMesh {
        glm::vec3 mapPosition;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> texCoords;
        glm::vec3 normal;
    };
    
    struct ChunkMesh {
        std::vector<MapVertex> ChunkVertecies;
        std::vector<uint32> ChunkIndices;
        ChunkArea2D ChunkArea;
    };
    
    //
    // Used in Voxel Map Index calculation
    // std::set comparison class called on each object in the set when using .find()
    //
    struct VertexCompareClass {
        bool operator() (const std::pair<Vertex, uint32>& p1, const std::pair<Vertex, uint32>& p2) const {
            if (fabs(p1.first.Position.x - p2.first.Position.x) > 0.001f) return p1.first.Position.x < p2.first.Position.x;
            if (fabs(p1.first.Position.y - p2.first.Position.y) > 0.001f) return p1.first.Position.y < p2.first.Position.y;
            if (fabs(p1.first.Position.z - p2.first.Position.z) > 0.001f) return p1.first.Position.z < p2.first.Position.z;
            if (fabs(p1.first.TextureCoords.x - p2.first.TextureCoords.x) > 0.001f) return p1.first.TextureCoords.x < p2.first.TextureCoords.x;
            if (fabs(p1.first.TextureCoords.y - p2.first.TextureCoords.y) > 0.001f) return p1.first.TextureCoords.y < p2.first.TextureCoords.y;
            if (fabs(p1.first.Normal.x - p2.first.Normal.x) > 0.001f) return p1.first.Normal.x < p2.first.Normal.x;
            if (fabs(p1.first.Normal.y - p2.first.Normal.y) > 0.001f) return p1.first.Normal.y < p2.first.Normal.y;
            if (fabs(p1.first.Normal.z - p2.first.Normal.z) > 0.001f) return p1.first.Normal.z < p2.first.Normal.z;
            return false;
        }
    };
    
}

#endif
