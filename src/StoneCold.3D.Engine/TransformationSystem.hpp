
#ifndef STONECOLD_TRANSFORMATIONSYSTEM_H
#define STONECOLD_TRANSFORMATIONSYSTEM_H

#include "EntityComponentSystem.hpp"

namespace StoneCold::Engine {

class TransformationSystem : public System {
public:
	//
	// Hardcoded System Component-Mask: 
	// Only Entities with a Transformation, Velocity and Collision Component will be updated with this System
	//
	TransformationSystem(EntityComponentSystem& ecs)
		: System((GetComponentMask<TransformationComponent>() | GetComponentMask<VelocityComponent>() | GetComponentMask<CollisionComponent>()))
		, _ecs(ecs) { }

	TransformationSystem(const TransformationSystem&) = delete;
	TransformationSystem& operator=(const TransformationSystem&) = delete;

	virtual void Update(uint64 frameTime) override {
		auto& transformComponents = *_ecs.GetComponentArray<TransformationComponent>();
		auto& velocityComponents = *_ecs.GetComponentArray<VelocityComponent>();
		auto& collisionComponents = *_ecs.GetComponentArray<CollisionComponent>();

		// Add a simple form of numerical integration (Explicit Euler) to speeds at different FPSs
		// (Explicit Euler works well as long as the speeds is constant or the frameTime is low)
		const float deltaSec = frameTime / 1000.0f;

		for (const auto& entityId : _entities) {
			auto& t = transformComponents[entityId];
			auto& v = velocityComponents[entityId];
			auto& c = collisionComponents[entityId];

			//// Normalize the velocity in case of diagonal movement
			//// FIND SOME WAY TO DO THIS WITHOUT IF TO NOT DISTURB THE CACHE, OUR LORD AND SAVIOR
			//if (v.Velocity.X != 0 || v.Velocity.Y != 0)
			//	v.Velocity.normalize();

			//// Update the position and round down to the next int
			//t.CurrentDelta.X = (v.Velocity.X * deltaSec) * t.Speed;
			//t.CurrentDelta.Y = (v.Velocity.Y * deltaSec) * t.Speed;
			//t.Position.X += t.CurrentDelta.X;
			//t.Position.Y += t.CurrentDelta.Y;

			//// Calculate the new CollisionBox based on the Hitbox and Position (Only works with smaller Hitboxes)
			//c.CollisionBox = {
			//	(t.Position.X + (floorf((t.Dimension.X - c.Hitbox.X) * t.Scale) / 2.f)),
			//	(t.Position.Y + (floorf((t.Dimension.Y - c.Hitbox.Y) * t.Scale) / 2.f)),
			//	c.Hitbox.X * t.Scale,
			//	c.Hitbox.Y * t.Scale
			//};
		}
	}

private:
	EntityComponentSystem& _ecs;
};

}

#endif
