
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

    struct VertexMapCube {
        glm::vec3 Position;
        glm::vec2 TextureCoords;
        glm::vec3 DiffuseLight;
    };
    typedef std::pair<std::vector<VertexMapCube>, std::vector<uint32>> MapMesh;
        
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

    struct MapMeshFace {
        glm::vec3 mapPosition;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> texcoords;
        glm::vec3 normal;
    };
    
    //
    // Used in Voxel Map Index calculation
    // std::set comparison class called on each object in the set when using .find()
    //
    typedef std::pair<Vertex, int> VertexIndexPair;
    struct VertexCompareClass {
        bool operator() (const VertexIndexPair& p1, const VertexIndexPair& p2) const {
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
