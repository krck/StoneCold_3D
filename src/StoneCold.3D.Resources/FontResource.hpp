
#ifndef STONECOLD_FONTRESOURCE_H
#define STONECOLD_FONTRESOURCE_H

#include "SDL_Base.hpp"
#include "Resource.hpp"

namespace StoneCold::Resources {

using namespace StoneCold::Base;

class FontResource : public Resource {
public:
	FontResource(const std::string& name
		, std::unique_ptr<TTF_Font, SDL_FontDeleter>&& fontSmall
		, std::unique_ptr<TTF_Font, SDL_FontDeleter>&& fontNormal
		, std::unique_ptr<TTF_Font, SDL_FontDeleter>&& fontBig)
		: Resource(name), _fontSmall(std::move(fontSmall)), _fontNormal(std::move(fontNormal)), _fontBig(std::move(fontBig)) { }

	//
	// GET, to pass around the raw pointer to the TTF_Font
	// (But the actual ownership stays with the Font object)
	//
	TTF_Font* GetFontSmall() const { return _fontSmall.get(); }
	TTF_Font* GetFontNormal() const { return _fontNormal.get(); }
	TTF_Font* GetFontBig() const { return _fontBig.get(); }

private:
	std::unique_ptr<TTF_Font, SDL_FontDeleter> _fontSmall;
	std::unique_ptr<TTF_Font, SDL_FontDeleter> _fontNormal;
	std::unique_ptr<TTF_Font, SDL_FontDeleter> _fontBig;

};

}

#endif
