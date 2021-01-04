
#ifndef STONECOLD_COLLISIONDETECTIONSYSTEM_H
#define STONECOLD_COLLISIONDETECTIONSYSTEM_H

#include <glm/vec2.hpp>
#include "EntityComponentSystem.hpp"

namespace StoneCold::Engine {

class CollisionDetectionSystem : public System {
public:
	//
	// Hardcoded System Component-Mask: 
	// Only Entities with a Collision Component will be updated with this System
	//
	CollisionDetectionSystem(EntityComponentSystem& ecs)
		: System((GetComponentMask<CollisionComponent>())), _ecs(ecs) { }

	CollisionDetectionSystem(const CollisionDetectionSystem&) = delete;
	CollisionDetectionSystem& operator=(const CollisionDetectionSystem&) = delete;

	virtual void Update(uint64 frameTime) override {
		auto& collisionComponents = *_ecs.GetComponentArray<CollisionComponent>();
		auto collisionComps = std::vector<CollisionComponent*>();
		collisionComps.reserve(_entities.size());
		for (const auto& e : _entities) {
			collisionComps.push_back(&collisionComponents[e]);
		}

		for (auto c_o : collisionComps) {
			//c_o->CollisionWith = nullptr;
			//// Check all Entitys with CollisionComponents against each other
			//for (auto c_i : collisionComps) {
			//	// Update the CollisionWith ptr, in case both objects had a collision (two fixed object will never)
			//	if (c_o->Tag != c_i->Tag && CalculateAABB(c_o->CollisionBox, c_i->CollisionBox)) {
			//		c_o->CollisionWith = c_i;
			//	}
			//}
		}
	}

private:
	//
	// Check the Axis-Aligned Bounding Boxes for overlap
	// Aka. AABB collision. Aka. simplest you can get
	//
	//inline bool CalculateAABB(const SDL_FRect& recA, const SDL_FRect& recB) const {
	//	return (recA.x + recA.w >= recB.x
	//		&& recB.x + recB.w >= recA.x
	//		&& recA.y + recA.h >= recB.y
	//		&& recB.y + recB.h >= recA.y);
	//}

private:
	EntityComponentSystem& _ecs;
};

}

#endif
