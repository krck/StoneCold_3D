
#ifndef STONECOLD_COMPONENTS_H
#define STONECOLD_COMPONENTS_H

#include <map>
#include <string>
#include <vector>
#include <glm/mat4x4.hpp>
#include "Types.hpp"

namespace StoneCold::Engine {

	using namespace StoneCold::Base;

	//
	// Basic Mesh component holding the IDs to the Vertex and Index data within the GPU
	// memory. Both IDs and the Indices size are needed to bind a Mesh for rendering
	//
	struct MeshComponent {
		uint32 VAO;
		uint32 EBO;
		uint64 Size;
	};

	struct TextureComponent {
		uint32 TextureId;
		uint32 NormalMapId;
		uint32 SpecularMapId;
	};

    struct InstanceComponent {
        uint64 InstanceCount;
    };
    
	//
	// TransformationComponent holding any Objects Position within the Game World
	//
	struct TransformationComponent {
		glm::mat4 Transformation;
	};

	//
	// VelocityComponent to move any Object within the Game World
	// 
	struct VelocityComponent {
		glm::vec3 Velocity;
		float Speed;
	};

	//struct AnimationComponent {
	//	const std::unordered_map<std::string, Animation>* Animations;
	//	const Animation* CurrentAnimation;
	//	uint32 CurrentFrameIndex;
	//	uint32 TimeElapsed;

	//	inline const Animation* GetAnimation(std::string name) {
	//		auto anim = &Animations->find(name)->second;
	//		// Reset the FrameIndex if the requested FrameCount is lower
	//		if (CurrentFrameIndex > anim->FrameCount)
	//			CurrentFrameIndex = 0;

	//		return anim;
	//	}
	//};

	//struct AttributeComponentUI {
	//	UiElementAttribute UiAttribute;
	//};

	//struct CollisionComponent {
	//	int Tag;	// USE ENUM TYPE
	//	Vec2 Hitbox;
	//	SDL_FRect CollisionBox;
	//	CollisionComponent* CollisionWith;
	//};

	//struct SpriteComponent {
	//	SDL_Texture* Texture;
	//	SDL_RendererFlip Flip;
	//};

	//struct SpriteLayeredComponent {
	//	SDL_Texture* TextureBottom;
	//	SDL_RendererFlip FlipBottom;
	//	SDL_Texture* TextureTop;
	//	SDL_RendererFlip FlipTop;
	//};

	//struct ScreenPositionComponent {
	//	SDL_Rect SrcRect;
	//	SDL_FRect DestRect;
	//};

	//struct ScreenPositionLayeredComponent {
	//	SDL_Rect SrcRectBottom;
	//	SDL_FRect DestRectBottom;
	//	SDL_Rect SrcRectTop;
	//	SDL_FRect DestRectTop;
	//};

	//struct EnemyStateComponent {
	//	const std::unordered_map<bitMask32, AiAction>* AiActions;
	//	bitMask32 AiDecisions;
	//};

}

#endif
