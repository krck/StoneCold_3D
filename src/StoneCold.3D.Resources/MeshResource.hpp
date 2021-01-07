
#ifndef STONECOLD_MESHRESOURCE_H
#define STONECOLD_MESHRESOURCE_H

#include <string>
#include <sstream>
#include <vector>
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>

#include "Resource.hpp"
#include "HelperClasses.hpp"
#include "TextureResource.hpp"

namespace StoneCold::Resources {

	using namespace StoneCold::Base;

	//
	// MeshResource
	//
	class MeshResource : public Resource {
	public:
		const std::string Directory;
		const std::vector<Vertex> Vertices;
		const std::vector<uint32> Indices;
		const TextureResource* Texture;

	public:
		MeshResource(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32>& indices, TextureResource* texture)
			: Resource(name), Vertices(vertices), Indices(indices), Texture(texture), _vao(0), _vbo(0), _ebo(0) {
			// Create the buffers
			glGenVertexArrays(1, &_vao);
			glGenBuffers(1, &_vbo);
			glGenBuffers(1, &_ebo);
			glBindVertexArray(_vao);
			// Load data into the vertex buffer
			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			glBufferData(GL_ARRAY_BUFFER, (sizeof(Vertex) * Vertices.size()), &Vertices[0], GL_STATIC_DRAW);
			// Set the vertex attribute pointers
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Tangent));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TextureCoords));
			glEnableVertexAttribArray(3);
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
