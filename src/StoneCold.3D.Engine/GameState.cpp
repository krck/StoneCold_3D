
#include "GameState.hpp"

using namespace StoneCold;
using namespace StoneCold::Engine;

GameState::GameState(uint16 maxEntities, EngineCore* engine, glm::mat4 gameProjection)
	: State(maxEntities, engine)
	, _messageService(MessageService::GetInstance())
	, _mapTiles(std::vector<entityId>())
    , _renderSystemMap(nullptr)
	, _renderSystemDefault(nullptr)
    , _transformationSystem(nullptr)
    , _renderSystemInstanced(nullptr)
	, _player(0)
	, _projection(gameProjection)
	, _camera(Camera(glm::vec3(200.0f, 100.0f, 200.f)))
	, _mouse(MouseClient(MouseServer::GetInstance()))
	, _keyboard(KeyboardClient(KeyboardServer::GetInstance())) { }


void GameState::Initialize() {
	// Create all Systems needed by the GameState ECS (keep ptr variables for quick access)
    _renderSystemMap = std::make_shared<RenderSystemMap>(_ecs);
	_renderSystemDefault = std::make_shared<RenderSystemDefault>(_ecs);
    _renderSystemInstanced = std::make_shared<RenderSystemInstanced>(_ecs);
	_transformationSystem = std::make_shared<TransformationSystem>(_ecs);

	// Add all the GameState Systems to the ECS
    _ecs.AddSystem<RenderSystemMap>(_renderSystemMap);
	_ecs.AddSystem<RenderSystemDefault>(_renderSystemDefault);
    _ecs.AddSystem<RenderSystemInstanced>(_renderSystemInstanced);
	_ecs.AddSystem<TransformationSystem>(_transformationSystem);
}


bool GameState::HandleMessages() {
	return true;
}


void GameState::HandleInputs() {


	//auto pos = camera.GetPosition();
	//std::cout << "X: " << pos.x << "\tY: " << pos.y << "\tZ: " << pos.z << std::endl;

	// Debug "dash"
	//t.Speed = (keyStates[SDL_SCANCODE_RCTRL] ? t.BaseSpeed * 3 : t.BaseSpeed);

	//// Update/Play a different Animation, based on the current input
	//if (t.Speed > t.BaseSpeed)
	//	// Dash/Dodge movement
	//	a.CurrentAnimation = a.GetAnimation("jump");
	//else if (v.Velocity.Y != 0.f || v.Velocity.X != 0.f)
	//	// Normal movement
	//	a.CurrentAnimation = a.GetAnimation("walk");
	//else
	//	// No movement
	//	a.CurrentAnimation = a.GetAnimation("idle");
}


void GameState::Update(uint64 frameTime) {
    const float deltaSec = frameTime / 1000.0f;
    // AS LONG AS THE CAMERA IS FIRST PERSON HANDLE INPUT HERE TO GET THE DELTA TIME
    if (_keyboard.IsKeyPressed(GLFW_KEY_A)) { _camera.ChangePosition(StoneCold::Engine::MoveDirection::LEFT, deltaSec); }
    if (_keyboard.IsKeyPressed(GLFW_KEY_D)) { _camera.ChangePosition(StoneCold::Engine::MoveDirection::RIGHT, deltaSec); }
    if (_keyboard.IsKeyPressed(GLFW_KEY_W)) { _camera.ChangePosition(StoneCold::Engine::MoveDirection::FORWARD, deltaSec); }
    if (_keyboard.IsKeyPressed(GLFW_KEY_S)) { _camera.ChangePosition(StoneCold::Engine::MoveDirection::BACKWARD, deltaSec); }
    if (_keyboard.IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) { _camera.ChangePosition(StoneCold::Engine::MoveDirection::DOWN, deltaSec); }
    if (_keyboard.IsKeyPressed(GLFW_KEY_SPACE)) { _camera.ChangePosition(StoneCold::Engine::MoveDirection::UP, deltaSec); }
    _camera.ChangeLineOfSight((int)_mouse.GetMouseX(), (int)_mouse.GetMouseY(), true);
    
	_transformationSystem->Update(frameTime);

	//// Center the camera over the Player
	//auto& t = _ecs.GetComponentArray<TransformationComponent>()->at(_player);
	//_camera.x = (t.Position.X + ((t.Dimension.X * t.Scale) / 2.f)) - (WINDOW_WIDTH / 2.f);
	//_camera.y = (t.Position.Y + ((t.Dimension.Y * t.Scale) / 2.f)) - (WINDOW_HEIGHT / 2.f);
}


void GameState::Render() {
    _renderSystemMap->Render(_projection, _camera.GetViewMatrix());
    _renderSystemInstanced->Render(_projection, _camera.GetViewMatrix());
	_renderSystemDefault->Render(_projection, _camera.GetViewMatrix());
}


void GameState::SetSpawnPosition(glm::u8vec2 spawnPoint) {
	// Update the players spawn point
	auto& t = _ecs.GetComponentArray<TransformationComponent>()->at(_player);
	//t.Position.X = static_cast<float>(spawnPoint.X);
	//t.Position.Y = static_cast<float>(spawnPoint.Y);
}


void GameState::SetEntities(entityId player, const std::vector<entityId>& mapTiles) {
	_player = player;
	_mapTiles = mapTiles;
}
