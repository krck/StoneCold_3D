
#ifndef STONECOLD_MESHLOADER_H
#define STONECOLD_MESHLOADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <memory>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Mesh.hpp"

namespace StoneCold::Resources {

	class MeshLoader {
	public:
		// Load simple, untextured mesh from an .obj File
		std::shared_ptr<Model> LoadSimpleModelFromFile(const std::string& path);

		//// Load complex Model: multiple Files, multiple Textures and Materials
		//std::shared_ptr<Model> LoadComplexModelFromFile(const std::string& path);

	private:
		//void processNode(aiNode* node, const aiScene* scene, const std::string& dir, std::vector<Mesh>& meshes);
		//Mesh processMesh(aiMesh* mesh, const aiScene* scene, const std::string& dir);

		//uint loadOrGetTexture(aiMaterial* material, const std::string& dir, aiTextureType type);
		//uint LoadTextureFromFile(const std::string& filename) const;

	private:
		//std::map<std::string, uint> texturesLoaded;
	};

}

#endif
