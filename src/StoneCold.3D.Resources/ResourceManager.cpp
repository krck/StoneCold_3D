
#include "ResourceManager.hpp"

using namespace StoneCold::Base;
using namespace StoneCold::Resources;

ResourceManager::ResourceManager()
	: _resources(std::unordered_map<std::string, std::shared_ptr<Resource>>())
	, _resouceLifetimes(std::unordered_map<ResourceLifeTime, std::vector<std::string>>())
	, _renderer(nullptr) {
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
	_basePath += "assets\\";
}


bool ResourceManager::Initialize(SDL_Renderer* renderer) {
	if (renderer != nullptr) {
		_renderer = renderer;

		_resouceLifetimes.insert({ ResourceLifeTime::Intro, std::vector<std::string>() });
		_resouceLifetimes.insert({ ResourceLifeTime::Game, std::vector<std::string>() });
		_resouceLifetimes.insert({ ResourceLifeTime::Menu, std::vector<std::string>() });
		_resouceLifetimes.insert({ ResourceLifeTime::Level, std::vector<std::string>() });

		return true;
	}
	else {
		return false;
	}
}


template<typename T>
T* ResourceManager::LoadExternalResource(ResourceLifeTime resourceLifeTime, const std::string& name) {
	try {
		// Load each ressource only once
		if (!IsResourceLoaded(name)) {
			if (std::is_same<T, TextureResource>::value) {
				// Create a Texture Resource, loaded from a File (.png or .jpg)
				_resources.insert({ name, std::make_shared<TextureResource>(CreateTexture(name)) });
				_resouceLifetimes[resourceLifeTime].push_back(name);
			}
			else if (std::is_same<T, FontResource>::value) {
				// Create a Font Resource loaded from a File (.ttf)
				_resources.insert({ name, std::make_shared<FontResource>(CreateFont(name)) });
				_resouceLifetimes[resourceLifeTime].push_back(name);
			}
		}
		return static_cast<T*>(_resources[name].get());
	}
	catch (...) {
		throw GameException("SDL Error on Resource creation: " + name
			+ "\n" + std::string(SDL_GetError())
			+ "\n" + std::string(IMG_GetError())
			+ "\n" + std::string(TTF_GetError()));
	}
}


TextureResource* ResourceManager::CreateFontTexture(ResourceLifeTime rlt, const std::string& name, TTF_Font* font, const std::string& text, const SDL_Color& color) {
	try {
		SDL_Rect srcRect = { 0, 0, 0, 0 };
		if (!IsResourceLoaded(name)) {
			// Create a Texture Resource, based on the Font, Text and Color
			SDL_Surface* tmpSurface = TTF_RenderText_Solid(font, text.c_str(), color);
			auto surfaceSize = Vec2i(tmpSurface->w, tmpSurface->h);
			auto tex = std::unique_ptr<SDL_Texture, SDL_TextureDeleter>(SDL_CreateTextureFromSurface(_renderer, tmpSurface));
			SDL_FreeSurface(tmpSurface);

			// Add the TextureResource to the map based on its Name and LifeTime
			_resources.insert({ name, std::make_shared<TextureResource>(TextureResource(name, std::move(tex), surfaceSize)) });
			_resouceLifetimes[rlt].push_back(name);
		}
		return static_cast<TextureResource*>(_resources[name].get());
	}
	catch (...) {
		throw GameException("SDL Error on Resource creation: " + name
			+ "\n" + std::string(SDL_GetError())
			+ "\n" + std::string(TTF_GetError()));
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


TextureResource ResourceManager::CreateTexture(const std::string& name) {
	auto fullPath = _basePath + name;

	// Load file into a SDL_Texture pointer
	SDL_Surface* tmpSurface = IMG_Load(fullPath.c_str());
	auto surfaceSize = Vec2i(tmpSurface->w, tmpSurface->h);
	auto tex = std::unique_ptr<SDL_Texture, SDL_TextureDeleter>(SDL_CreateTextureFromSurface(_renderer, tmpSurface));
	SDL_FreeSurface(tmpSurface);

	// Create the TextureResource
	return TextureResource(name, std::move(tex), surfaceSize);
}


FontResource ResourceManager::CreateFont(const std::string& name) {
	auto fullPath = (_basePath + name);

	// Create managed Fonts ptrs, in the 3 different sizes (Fontsize must be set on load)
	auto fontSmall = std::unique_ptr<TTF_Font, SDL_FontDeleter>(TTF_OpenFont(fullPath.c_str(), FONT_SIZE_SMALL));
	auto fontNormal = std::unique_ptr<TTF_Font, SDL_FontDeleter>(TTF_OpenFont(fullPath.c_str(), FONT_SIZE_NORMAL));
	auto fontBig = std::unique_ptr<TTF_Font, SDL_FontDeleter>(TTF_OpenFont(fullPath.c_str(), FONT_SIZE_BIG));

	return FontResource(name, std::move(fontSmall), std::move(fontNormal), std::move(fontBig));
}


//
// Explicitly instanciate every form of LoadExternalResource
// This has two upsides:
// - .hpp will not get cluttered with function definitions
// - There are not many Resource-Types so its easy to provide a concrete interface
//
template TextureResource* ResourceManager::LoadExternalResource<TextureResource>(ResourceLifeTime resourceLifeTime, const std::string& name);
template FontResource* ResourceManager::LoadExternalResource<FontResource>(ResourceLifeTime resourceLifeTime, const std::string& name);
