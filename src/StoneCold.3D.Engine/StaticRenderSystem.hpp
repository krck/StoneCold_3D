
#ifndef STONECOLD_STATICRENDERSYSTEM_H
#define STONECOLD_STATICRENDERSYSTEM_H

#include "EntityComponentSystem.hpp"

namespace StoneCold::Engine {

class StaticRenderSystem : public System {
public:
	//
	// Hardcoded System Component-Mask: 
	// Only Entities with a RENDER_STATIC Flag, a ScreenPosition and a Sprite Component will be rendered with this System
	//
	StaticRenderSystem(SDL_Renderer* renderer, EntityComponentSystem& ecs)
		: System((RENDER_STATIC | GetComponentMask<ScreenPositionComponent>() | GetComponentMask<SpriteComponent>()))
		, _renderer(renderer), _ecs(ecs) { }

	StaticRenderSystem(const StaticRenderSystem&) = delete;
	StaticRenderSystem& operator=(const StaticRenderSystem&) = delete;

	virtual void Render(SDL_FRect camera) override {
		auto& positionComponents = *_ecs.GetComponentArray<ScreenPositionComponent>();
		auto& spriteComponents = *_ecs.GetComponentArray<SpriteComponent>();

		for (const auto& entityId : _entities) {
			auto& p = positionComponents[entityId];
			auto& s = spriteComponents[entityId];

			// Get the Destination rectangle (render position on screen) relative to the camera (as int)
			SDL_FRect currentDest = {
				floorf(p.DestRect.x - camera.x),
				floorf(p.DestRect.y - camera.y),
				floorf(p.DestRect.w),
				floorf(p.DestRect.h)
			};

			// Add Sprite to the SDL Renderer
			SDL_RenderCopyExF(_renderer, s.Texture, &p.SrcRect, &currentDest, 0, nullptr, s.Flip);
		}
	}

private:
	SDL_Renderer* _renderer;
	EntityComponentSystem& _ecs;
};

}

#endif
