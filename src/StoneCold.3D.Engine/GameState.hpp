
#ifndef STONECOLD_GAMESTATE_H
#define STONECOLD_GAMESTATE_H

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include "Settings.hpp"
#include "EngineCore.hpp"
#include "RenderSystemDefault.hpp"
#include "RenderSystemInstanced.hpp"
#include "RenderSystemDefaultNoTex.hpp"
#include "TransformationSystem.hpp"
#include "Camera.hpp"

namespace StoneCold::Engine {

class GameState : public State {
public:
	GameState(uint16 maxEntities, EngineCore* engine, glm::mat4 gameProjection);
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
	glm::mat4 _projection;
	// System ptrs for fast access
	std::shared_ptr<RenderSystemDefaultNoTex> _renderDefaultNoTex;
    std::shared_ptr<RenderSystemInstanced> _renderSystemInstanced;
	std::shared_ptr<RenderSystemDefault> _renderDefault;
	std::shared_ptr<TransformationSystem> _transformationSystem;
};

}

#endif
