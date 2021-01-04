
#ifndef STONECOLD_COLLISIONRESOLUTIONSYSTEM_H
#define STONECOLD_COLLISIONRESOLUTIONSYSTEM_H

#include "EntityComponentSystem.hpp"

namespace StoneCold::Engine {

class CollisionResolutionSystem : public System {
public:
	//
	// Hardcoded System Component-Mask: 
	// Only Entities with a Collision and Transformation Component will be updated with this System
	//
	CollisionResolutionSystem(EntityComponentSystem& ecs)
		: System((GetComponentMask<CollisionComponent>() | GetComponentMask<TransformationComponent>())), _ecs(ecs) { }

	CollisionResolutionSystem(const CollisionResolutionSystem&) = delete;
	CollisionResolutionSystem& operator=(const CollisionResolutionSystem&) = delete;

	virtual void Update(uint64 frameTime) override {
		auto& collisionComponents = *_ecs.GetComponentArray<CollisionComponent>();
		auto& transformComponents = *_ecs.GetComponentArray<TransformationComponent>();

		for (const auto& entityId : _entities) {
			auto& c = collisionComponents[entityId];
			auto& t = transformComponents[entityId];

			//if (c.CollisionWith != nullptr) {
			//	t.Position.X -= (t.CurrentDelta.X * 3.f);
			//	t.Position.Y -= (t.CurrentDelta.Y * 3.f);
			//}
		}
	}

private:
	EntityComponentSystem& _ecs;
};

}

#endif
