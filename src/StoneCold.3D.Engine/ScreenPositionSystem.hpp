
#ifndef STONECOLD_SCREENPOSITIONSYSTEM_H
#define STONECOLD_SCREENPOSITIONSYSTEM_H

#include "EntityComponentSystem.hpp"

namespace StoneCold::Engine {

class ScreenPositionSystem : public System {
public:
	//
	// Hardcoded System Component-Mask: 
	// Only Entities with a Velocity, ScreenPosition and Sprite Component will be updated with this System
	//
	ScreenPositionSystem(EntityComponentSystem& ecs)
		: System((GetComponentMask<VelocityComponent>() | GetComponentMask<TransformationComponent>() | GetComponentMask<ScreenPositionComponent>()))
		, _ecs(ecs) { }

	ScreenPositionSystem(const ScreenPositionSystem&) = delete;
	ScreenPositionSystem& operator=(const ScreenPositionSystem&) = delete;

	virtual void Update(uint64 frameTime) override {
		auto& transformComponents = *_ecs.GetComponentArray<TransformationComponent>();
		auto& positionComponents = *_ecs.GetComponentArray<ScreenPositionComponent>();

		for (const auto& entityId : _entities) {
			auto& t = transformComponents[entityId];
			auto& p = positionComponents[entityId];

			// Update the Sprite position on screen based on the transformations
			//p.DestRect.x = t.Position.X;
			//p.DestRect.y = t.Position.Y;
			//p.DestRect.w = t.Dimension.X * t.Scale;
			//p.DestRect.h = t.Dimension.Y * t.Scale;
		}
	}

private:
	EntityComponentSystem& _ecs;
};

}

#endif
