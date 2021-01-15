
#ifndef STONECOLD_RESOURCEMANAGER_H
#define STONECOLD_RESOURCEMANAGER_H

#ifdef _WIN32
#include <windows.h>
#endif

#include <unordered_map>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <memory>

#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "StoneColdBase.hpp"
#include "Enums.hpp"
#include "Settings.hpp"
#include "Exception.hpp"
#include "MeshResource.hpp"
#include "ModelResource.hpp"
#include "TextureResource.hpp"

namespace StoneCold::Resources {

	using namespace StoneCold::Base;

	//
	// ResorceManager to load and unlaod external Resources (from the Filesystem)
	// - Ensures that only one copy of each unique resource exists
	// - Manages the lifetime of each resource (loading / unloading)
	// - !NOT YET! Handles loading of composite resources (resource dependencies)
	//
	class ResourceManager {
	public:
		ResourceManager();
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;

		bool Initialize();

		//
		// Load any Resource based on its Type
		// Ensures that Resources are loaded only once
		//
		template<typename T>
		T* LoadResource(ResourceLifeTime resourceLifeTime, const std::string& name);

		//
		// Unload (cleanup) all Resources of a specific LifeTime
		//
		void UnloadResources(ResourceLifeTime resourceLifeTime);

		template<typename T>
		inline T* GetResource(const std::string& name) { return (_resources.find(name) != _resources.end() ? static_cast<T*>(_resources[name].get()) : nullptr); }
		inline bool IsResourceLoaded(const std::string& name) const { return (_resources.find(name) != _resources.end()); }

		~ResourceManager() = default;

	private:
		std::shared_ptr<MeshResource> LoadMeshResource(const std::string& name);
		std::shared_ptr<ModelResource> LoadModelResource(const std::string& name, ResourceLifeTime resourceLifeTime);
		std::shared_ptr<TextureResource> LoadTextureResource(const std::string& name);

		void ProcessAssimpNode(aiNode* node, const aiScene* scene, const std::string& dir, std::vector<MeshResource>& meshes, ResourceLifeTime resourceLifeTime);
		MeshResource ProcessAssimpMesh(aiMesh* mesh, const aiScene* scene, const std::string& dir, ResourceLifeTime resourceLifeTime);
		uint32 ProcessAssimpTexture(aiMaterial* material, const std::string& dir, aiTextureType type) const;

	private:
		std::string _basePath;
		std::unordered_map<std::string, std::shared_ptr<Resource>> _resources;
		std::unordered_map<ResourceLifeTime, std::vector<std::string>> _resouceLifetimes;
	};

}

#endif
