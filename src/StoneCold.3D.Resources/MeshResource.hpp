
#ifndef STONECOLD_MESHRESOURCE_H
#define STONECOLD_MESHRESOURCE_H

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
	// MeshResource
	//
	class MeshResource : public Resource {
	public:
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
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, BiTangent));
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TextureCoords));
			glEnableVertexAttribArray(4);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// Load data into the index buffer
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (sizeof(uint32) * indices.size()), &indices[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
        
        //
        // Add additional Instance Array Buffer
        //
        // In case one Mesh needs to be rendered thousands or ten-thousands of times
        // add another Buffer with transformation matrices for each of the instances
        //
        void AddInstanceArray(const std::vector<glm::mat4>& instances) {
            if(instances.size()) {
                _instanceCount = instances.size();
                
                glGenBuffers(1, &_instanceBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, _instanceBuffer);
                glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::mat4) * instances.size()), &instances[0], GL_STATIC_DRAW);
                
                // Set transformation matrices as an instance vertex attribute (4 times vec4 with divisor 1)
                glBindVertexArray(_vao);
                glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);
                glEnableVertexAttribArray(5);
                glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)));
                glEnableVertexAttribArray(6);
                glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(2 * sizeof(glm::vec4)));
                glEnableVertexAttribArray(7);
                glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(3 * sizeof(glm::vec4)));
                glEnableVertexAttribArray(8);

                // Divisor is set to tell OpenGL when to update the content of the Vertex Shader. With this
                // set to 1 the Vertex Shader Attributes will be updated on every render of a new instance
                glVertexAttribDivisor(5, 1);
                glVertexAttribDivisor(6, 1);
                glVertexAttribDivisor(7, 1);
                glVertexAttribDivisor(8, 1);

                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindVertexArray(0);
            }
        }

		inline uint32 GetVAO() const { return _vao; }
		inline uint32 GetEBO() const { return _ebo; }
		inline uint64 GetSize() const { return Indices.size(); }
        inline uint64 GetInstanceCount() const { return _instanceCount; }

	private:
		uint32 _vao;
		uint32 _vbo;
		uint32 _ebo;
        uint32 _instanceBuffer;
        uint64 _instanceCount;
	};

}

#endif
