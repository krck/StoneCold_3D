
#ifndef STONECOLD_TRANSFORMATIONSYSTEM_H
#define STONECOLD_TRANSFORMATIONSYSTEM_H

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "EntityComponentSystem.hpp"

namespace StoneCold::Engine {

	class TransformationSystem : public System {
	public:
		//
		// Hardcoded System Component-Mask: 
		// Only Entities with a Transformation and Velocity component will be updated with this System
		//
		TransformationSystem(EntityComponentSystem& ecs)
			: System((GetComponentMask<TransformationComponent>() | GetComponentMask<VelocityComponent>()))
			, _ecs(ecs) { }

		TransformationSystem(const TransformationSystem&) = delete;
		TransformationSystem& operator=(const TransformationSystem&) = delete;

		virtual void Update(uint64 frameTime) override {
			auto& transformComponents = *_ecs.GetComponentArray<TransformationComponent>();
			auto& velocityComponents = *_ecs.GetComponentArray<VelocityComponent>();

			// Add a simple form of numerical integration (Explicit Euler) to speeds at different FPSs
			// (Explicit Euler works well as long as the speeds is constant or the frameTime is low)
			const float deltaSec = frameTime / 1000.0f;

			for (const auto& entityId : _entities) {
				auto& t = transformComponents[entityId];
				auto& v = velocityComponents[entityId];

				// Normalize the velocity in case of diagonal movement
				v.Velocity = glm::normalize(v.Velocity);

				// Update the position
				//t.Position.x += (v.Velocity.x * deltaSec) * v.Speed;
				//t.Position.y += (v.Velocity.y * deltaSec) * v.Speed;
				//t.Position.z += (v.Velocity.z * deltaSec) * v.Speed;
				// glm::translate(t.Transformation, t.Position);

				// Calculate the new CollisionBox based on the Hitbox and Position (Only works with smaller Hitboxes)
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
