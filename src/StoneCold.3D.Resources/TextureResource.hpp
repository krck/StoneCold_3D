
#ifndef STONECOLD_TEXTURERESOURCES_H
#define STONECOLD_TEXTURERESOURCES_H

#include "SDL_Base.hpp"
#include "Resource.hpp"
#include "Vec2.hpp"

namespace StoneCold::Resources {

using namespace StoneCold::Base;

class TextureResource : public Resource {
public:
	TextureResource(const std::string& name, std::unique_ptr<SDL_Texture, SDL_TextureDeleter>&& textureSDL, Vec2i surfaceSize)
		: Resource(name), _textureSDL(std::move(textureSDL)), SurfaceSize(surfaceSize) { }

	//
	// GET, to pass around the raw pointer to the SDL_Texture
	// (But the actual ownership stays with the Texture object)
	//
	SDL_Texture* GetTextureSDL() const { return _textureSDL.get(); }

public:
	//
	// Width and Height of the original Image (SDL_Surface)
	//
	const Vec2i SurfaceSize;
private:
	std::unique_ptr<SDL_Texture, SDL_TextureDeleter> _textureSDL;
};

}

#endif
