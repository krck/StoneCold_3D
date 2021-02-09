
#ifndef STONECOLD_MAPRESOURCE_H
#define STONECOLD_MAPRESOURCE_H

#include <string>
#include <sstream>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "Resource.hpp"
#include "HelperClasses.hpp"
#include "TextureResource.hpp"

namespace StoneCold::Resources {

    using namespace StoneCold::Base;

    //
    // MapResource
    //
    class MapResource : public Resource {
    public:
        const std::vector<MapVertex> Vertices;
        const std::vector<uint32> Indices;

    public:
        MapResource(const std::string& name, const std::vector<MapVertex>& vertices, const std::vector<uint32>& indices)
            : Resource(name), Vertices(vertices), Indices(indices), _vao(0), _vbo(0), _ebo(0) {
            // Create the buffers
            glGenVertexArrays(1, &_vao);
            glGenBuffers(1, &_vbo);
            glGenBuffers(1, &_ebo);
            // Load data into the vertex buffer
            glBindVertexArray(_vao);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glBufferData(GL_ARRAY_BUFFER, (sizeof(MapVertex) * Vertices.size()), &Vertices[0], GL_STATIC_DRAW);
            // Set the vertex attribute pointers
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MapVertex), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(MapVertex), (GLvoid*)offsetof(MapVertex, TextureCoords));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(MapVertex), (GLvoid*)offsetof(MapVertex, DiffuseLight));
            glEnableVertexAttribArray(2);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // Load data into the index buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, (sizeof(uint32) * indices.size()), &indices[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        inline uint32 GetVAO() const { return _vao; }
        inline uint32 GetEBO() const { return _ebo; }
        inline uint64 GetSize() const { return Indices.size(); }

    private:
        uint32 _vao;
        uint32 _vbo;
        uint32 _ebo;
    };

}

#endif
