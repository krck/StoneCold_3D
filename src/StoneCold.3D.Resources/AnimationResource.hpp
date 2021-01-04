
#ifndef STONECOLD_ANIMATIONRESOURCE_H
#define STONECOLD_ANIMATIONRESOURCE_H

#include "SDL_Base.hpp"
#include "Resource.hpp"
#include <utility>

namespace StoneCold::Resources {

//
// Animation contains only its name and an array of source rectangles (SDL_Rect srcRects) 
// to define which part of a full Texture image is part of the Animation / should be rendered
//
struct Animation {
	Animation(const std::vector<SDL_Rect>& fp, uint32 ft) : FramePositions(fp), FrameCount(fp.size() - 1), FrameTime(ft) {}

	const std::vector<SDL_Rect> FramePositions;
	const size_t FrameCount;
	const uint32 FrameTime;
};

class AnimationResource : public Resource {
public:
	AnimationResource(const std::string& name, const std::unordered_map<std::string, Animation>* animations)
		:  Resource(name), Animations(animations) {}

public:
	const std::unordered_map<std::string, Animation>* Animations;
};

}

#endif
