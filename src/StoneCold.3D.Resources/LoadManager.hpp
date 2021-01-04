
#ifndef STONECOLD_LOADMANAGER_H
#define STONECOLD_LOADMANAGER_H

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

#include "MeshResource.hpp"

namespace StoneCold::Resources {

	class LoadManager {
	public:
		// Load simple, untextured mesh from an .obj File
		MeshResource LoadSimpleMeshFromFile(const std::string name, const std::string& fullPath);

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
