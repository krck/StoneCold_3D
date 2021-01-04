
#include "ResourceManager.hpp"

using namespace StoneCold::Base;
using namespace StoneCold::Resources;

ResourceManager::ResourceManager()
	: _resources(std::unordered_map<std::string, std::shared_ptr<Resource>>())
	, _resouceLifetimes(std::unordered_map<ResourceLifeTime, std::vector<std::string>>()) {
	// Get the current path to the running .exe
	// When NULL is passed to GetModuleHandle, the handle of the exe itself is returned
	char ownPth[MAX_PATH];
	HMODULE hModule = GetModuleHandle(NULL);
	if (hModule != NULL) {
		GetModuleFileName(hModule, ownPth, (sizeof(ownPth)));
	}

	// Create the relative path to the current assets folder
	_basePath = std::string(ownPth);
	_basePath = _basePath.substr(0, _basePath.find_last_of("\\") + 1);
	_basePath += "res\\";
}


bool ResourceManager::Initialize(LoadManager* loadManager) {
	if (loadManager != nullptr) {
		_loadManager = loadManager;
		_resouceLifetimes.insert({ ResourceLifeTime::Intro, std::vector<std::string>() });
		_resouceLifetimes.insert({ ResourceLifeTime::Game, std::vector<std::string>() });
		_resouceLifetimes.insert({ ResourceLifeTime::Menu, std::vector<std::string>() });
		_resouceLifetimes.insert({ ResourceLifeTime::Level, std::vector<std::string>() });
		return true;
	}
	return false;
}


template<typename T>
T* ResourceManager::LoadExternalResource(ResourceLifeTime resourceLifeTime, const std::string& name) {
	try {
		// Load each ressource only once
		if (!IsResourceLoaded(name)) {
			if (std::is_same<T, MeshResource>::value) {
				// Create a simple Model Resource, loaded from a File (.obj, no Texture)
				_resources.insert({ name, LoadMeshResource(name) });
				_resouceLifetimes[resourceLifeTime].push_back(name);
			}
			else if (std::is_same<T, ModelResource>::value) {
				// Create a complex Model Resource, loaded from a File (.obj, textures, multiple meshes)
				_resources.insert({ name, LoadModelResource(name) });
				_resouceLifetimes[resourceLifeTime].push_back(name);
			}
			else if (std::is_same<T, TextureResource>::value) {
				// Create a Texture Resource, loaded from a File (.png or .jpg)
				_resources.insert({ name, LoadTextureResource(name) });
				_resouceLifetimes[resourceLifeTime].push_back(name);
			}
		}
		return static_cast<T*>(_resources[name].get());
	}
	catch (std::exception ex) {
		throw GameException("Error on Resource creation: " + name + "\n" + ex.what());
	}
}


void ResourceManager::UnloadExternalResources(ResourceLifeTime resourceLifeTime) {
	// Remove all Resources that are mapped to the specific lifetime
	const auto& keys = _resouceLifetimes[resourceLifeTime];
	for (const auto& key : keys) {
		_resources.erase(key);
	}

	// Clear all ResourceLifeTime keys
	_resouceLifetimes[resourceLifeTime] = std::vector<std::string>();
}


std::shared_ptr<MeshResource> ResourceManager::LoadMeshResource(const std::string& name) {
	auto fullPath = (_basePath + name);
	return std::make_shared<MeshResource>(_loadManager->LoadSimpleMeshFromFile(name, fullPath));
}


std::shared_ptr<ModelResource> ResourceManager::LoadModelResource(const std::string& name) {
	auto fullPath = (_basePath + name);
	return nullptr;
}


std::shared_ptr<TextureResource> ResourceManager::LoadTextureResource(const std::string& name) {
	auto fullPath = _basePath + name;
	return nullptr;
}


//
// Explicitly instanciate every form of LoadExternalResource
// This has two upsides:
// - .hpp will not get cluttered with function definitions
// - There are not many Resource-Types so its easy to provide a concrete interface
//
template MeshResource* ResourceManager::LoadExternalResource<MeshResource>(ResourceLifeTime resourceLifeTime, const std::string& name);
template TextureResource* ResourceManager::LoadExternalResource<TextureResource>(ResourceLifeTime resourceLifeTime, const std::string& name);
