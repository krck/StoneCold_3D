
#ifndef STONECOLD_GAMESTATE_H
#define STONECOLD_GAMESTATE_H

#include <glm/vec2.hpp>

#include "Settings.hpp"
#include "EngineCore.hpp"
#include "AnimationSystem.hpp"
#include "TransformationSystem.hpp"
#include "CollisionDetectionSystem.hpp"
#include "CollisionResolutionSystem.hpp"
#include "ScreenPositionSystem.hpp"
#include "StaticRenderSystem.hpp"
#include "MotionRenderSystem.hpp"
#include "Camera.hpp"

namespace StoneCold::Engine {

class GameState : public State {
public:
	GameState(uint16 maxEntities, EngineCore* engine);
	GameState(const GameState&) = delete;
	GameState& operator=(const GameState&) = delete;

	virtual void Initialize() override;

	virtual bool HandleMessages() override;
	virtual void HandleInputs() override;
	virtual void Update(uint64 frameTime) override;
	virtual void Render() override;

	void SetSpawnPosition(glm::u8vec2 spawnPoint);
	void SetEntities(entityId player, const std::vector<entityId>& mapTiles);

	inline const std::vector<entityId>& GetMapTiles() { return _mapTiles; }

	~GameState() = default;

private:
	Camera _camera;
	MouseClient _mouse;
	KeyboardClient _keyboard;
	MessageService* _messageService;
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
