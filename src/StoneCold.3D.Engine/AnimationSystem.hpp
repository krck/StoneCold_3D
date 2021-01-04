
#ifndef STONECOLD_ANIMATIONSYSTEM_H
#define STONECOLD_ANIMATIONSYSTEM_H

#include "EntityComponentSystem.hpp"

namespace StoneCold::Engine {

class AnimationSystem : public System {
public:
	//
	// Hardcoded System Component-Mask: 
	// Only Entities with a Animation and Screen-Position Component will be updated with this System
	//
	AnimationSystem(EntityComponentSystem& ecs)
		: System((GetComponentMask<AnimationComponent>() | GetComponentMask<ScreenPositionComponent>()))
		, _ecs(ecs) { }

	AnimationSystem(const AnimationSystem&) = delete;
	AnimationSystem& operator=(const AnimationSystem&) = delete;

	virtual void Update(uint32 frameTime) override {
		auto& animationComponents = *_ecs.GetComponentArray<AnimationComponent>();
		auto& positionComponents = *_ecs.GetComponentArray<ScreenPositionComponent>();

		for (const auto& entityId : _entities) {
			auto& a = animationComponents[entityId];
			auto& p = positionComponents[entityId];

			// Udpate the Animation index based on its "play-speed" in case its set to automaticUpdate
			// FIND SOME WAY TO DO THIS WITHOUT IFs TO NOT DISTURB THE CACHE, OUR LORD AND SAVIOR
			a.TimeElapsed += frameTime;
			if (a.TimeElapsed > a.CurrentAnimation->FrameTime) {
				a.TimeElapsed -= a.CurrentAnimation->FrameTime;
				a.CurrentFrameIndex = (a.CurrentFrameIndex < a.CurrentAnimation->FrameCount ? a.CurrentFrameIndex + 1 : 0);
			}

			p.SrcRect = a.CurrentAnimation->FramePositions[a.CurrentFrameIndex];
		}
	}

private:
	EntityComponentSystem& _ecs;
};

}

#endif
