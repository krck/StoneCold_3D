
#ifndef STONECOLD_COMPONENTS_H
#define STONECOLD_COMPONENTS_H

#include "Types.hpp"
#include <unordered_map>
#include <vector>

namespace StoneCold::Engine {

	using namespace StoneCold::Base;

	struct AnimationComponent {
		//const std::unordered_map<std::string, Animation>* Animations;
		//const Animation* CurrentAnimation;
		//uint32 CurrentFrameIndex;
		//uint32 TimeElapsed;

		//inline const Animation* GetAnimation(std::string name) {
		//	auto anim = &Animations->find(name)->second;
		//	// Reset the FrameIndex if the requested FrameCount is lower
		//	if (CurrentFrameIndex > anim->FrameCount)
		//		CurrentFrameIndex = 0;

		//	return anim;
		//}
	};

	struct AttributeComponentUI {
		//UiElementAttribute UiAttribute;
	};

	struct CollisionComponent {
		//int Tag;	// USE ENUM TYPE
		//Vec2 Hitbox;
		//SDL_FRect CollisionBox;
		//CollisionComponent* CollisionWith;
	};

	struct SpriteComponent {
		//SDL_Texture* Texture;
		//SDL_RendererFlip Flip;
	};

	struct SpriteLayeredComponent {
		//SDL_Texture* TextureBottom;
		//SDL_RendererFlip FlipBottom;
		//SDL_Texture* TextureTop;
		//SDL_RendererFlip FlipTop;
	};

	struct TransformationComponent {
		//Vec2 Position;
		//Vec2 CurrentDelta;
		//Vec2 Dimension;
		//uint32 BaseSpeed;
		//uint32 Speed;
		//uint32 Scale;
	};

	struct VelocityComponent {
		//Vec2 Velocity;
	};

	struct ScreenPositionComponent {
		//SDL_Rect SrcRect;
		//SDL_FRect DestRect;
	};

	struct ScreenPositionLayeredComponent {
		//SDL_Rect SrcRectBottom;
		//SDL_FRect DestRectBottom;
		//SDL_Rect SrcRectTop;
		//SDL_FRect DestRectTop;
	};

	struct EnemyStateComponent {
		//const std::unordered_map<bitMask32, AiAction>* AiActions;
		//bitMask32 AiDecisions;
	};

}

#endif
