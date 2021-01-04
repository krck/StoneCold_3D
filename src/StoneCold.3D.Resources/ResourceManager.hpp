
#ifndef STONECOLD_RESOURCEMANAGER_H
#define STONECOLD_RESOURCEMANAGER_H

#include <windows.h>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include "Enums.hpp"
#include "Settings.hpp"
#include "Exception.hpp"
#include "MeshResource.hpp"
#include "ModelResource.hpp"
#include "TextureResource.hpp"
#include "LoadManager.hpp"

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

		bool Initialize(LoadManager* loadManager);

		//
		// Load any Resource based on its Type
		// Ensures that Resources are loaded only once
		//
		template<typename T>
		T* LoadExternalResource(ResourceLifeTime resourceLifeTime, const std::string& name);

		//
		// Unload (cleanup) all Resources of a specific LifeTime
		//
		void UnloadExternalResources(ResourceLifeTime resourceLifeTime);

		template<typename T>
		inline T* GetResource(const std::string& name) { return (_resources.find(name) != _resources.end() ? static_cast<T*>(_resources[name].get()) : nullptr); }
		inline bool IsResourceLoaded(const std::string& name) const { return (_resources.find(name) != _resources.end()); }

		~ResourceManager() = default;

	private:
		std::shared_ptr<MeshResource> LoadMeshResource(const std::string& name);
		std::shared_ptr<ModelResource> LoadModelResource(const std::string& name);
		std::shared_ptr<TextureResource> LoadTextureResource(const std::string& name);

	private:
		std::string _basePath;
		std::unordered_map<std::string, std::shared_ptr<Resource>> _resources;
		std::unordered_map<ResourceLifeTime, std::vector<std::string>> _resouceLifetimes;
		LoadManager* _loadManager;
	};

}

#endif
