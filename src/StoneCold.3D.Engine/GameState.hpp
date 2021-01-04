
#ifndef STONECOLD_GAMESTATE_H
#define STONECOLD_GAMESTATE_H

#include "Settings.hpp"
#include "EngineCore.hpp"
#include "EventManager.hpp"
#include "AnimationSystem.hpp"
#include "TransformationSystem.hpp"
#include "CollisionDetectionSystem.hpp"
#include "CollisionResolutionSystem.hpp"
#include "ScreenPositionSystem.hpp"
#include "StaticRenderSystem.hpp"
#include "MotionRenderSystem.hpp"

namespace StoneCold::Engine {

class GameState : public State {
public:
	GameState(uint16 maxEntities, SDL_Renderer* renderer, EngineCore* engine);
	GameState(const GameState&) = delete;
	GameState& operator=(const GameState&) = delete;

	virtual void Initialize() override;

	virtual bool HandleSDLEvent(const SDL_Event& sdlEvent) override;
	virtual void HandleInputEvent(const std::vector<uint8>& keyStates) override;
	virtual void Update(uint32 frameTime) override;
	virtual void Render() override;

	void SetSpawnPosition(Vec2i spawnPoint);
	void SetEntities(entityId player, const std::vector<entityId>& mapTiles);

	inline const std::vector<entityId>& GetMapTiles() { return _mapTiles; }

	~GameState() = default;

private:
	SDL_FRect _camera;
	EventManager& _eventManager;
	// EntityId's for fast access
	entityId _player;
	std::vector<entityId> _mapTiles;
	// System ptrs for fast access
	std::shared_ptr<AnimationSystem> _animationSystem;
	std::shared_ptr<TransformationSystem> _transformationSystem;
	std::shared_ptr<CollisionDetectionSystem> _collisionDetectionSystem;
	std::shared_ptr<CollisionResolutionSystem> _collisionResolutionSystem;
	std::shared_ptr<ScreenPositionSystem> _screenPositionSystem;
	std::shared_ptr<StaticRenderSystem> _staticRenderSystem;
	std::shared_ptr<MotionRenderSystem> _motionRenderSystem;
};

}

#endif
