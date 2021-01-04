
#ifndef STONECOLD_LAYEREDRENDERSYSTEM_H
#define STONECOLD_LAYEREDRENDERSYSTEM_H

#include "EntityComponentSystem.hpp"

namespace StoneCold::Engine {

	class LayeredRenderSystem : public System {
	public:
		//
		// Hardcoded System Component-Mask: 
		// Only Entities with a layered ScreenPosition and a layered Sprite Component will be rendered with this System
		//
		LayeredRenderSystem(SDL_Renderer* renderer, EntityComponentSystem& ecs)
			: System((GetComponentMask<ScreenPositionLayeredComponent>() | GetComponentMask<SpriteLayeredComponent>()))
			, _renderer(renderer), _ecs(ecs) { }

		LayeredRenderSystem(const LayeredRenderSystem&) = delete;
		LayeredRenderSystem& operator=(const LayeredRenderSystem&) = delete;

		virtual void Render(SDL_FRect camera) override {
			auto& positionLayerComponents = *_ecs.GetComponentArray<ScreenPositionLayeredComponent>();
			auto& spriteLayerComponents = *_ecs.GetComponentArray<SpriteLayeredComponent>();

			for (const auto& entityId : _entities) {
				auto& p = positionLayerComponents[entityId];
				auto& s = spriteLayerComponents[entityId];

				// Add Sprite to the SDL Renderer
				SDL_RenderCopyF(_renderer, s.TextureBottom, &p.SrcRectBottom, &p.DestRectBottom);
				SDL_RenderCopyF(_renderer, s.TextureTop, &p.SrcRectTop, &p.DestRectTop);
			}
		}

	private:
		SDL_Renderer* _renderer;
		EntityComponentSystem& _ecs;
	};

}

#endif
