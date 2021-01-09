
#include "GameState.hpp"

using namespace StoneCold;
using namespace StoneCold::Engine;

GameState::GameState(uint16 maxEntities, EngineCore* engine, glm::mat4 gameProjection)
	: State(maxEntities, engine)
	, _messageService(MessageService::GetInstance())
	, _mapTiles(std::vector<entityId>())
	, _renderDefault(nullptr)
	, _renderDefaultNoTex(nullptr)
	, _transformationSystem(nullptr)
	, _player(0)
	, _projection(gameProjection)
	, _camera(Camera(glm::vec3(0.0f, 10.0f, 10.f)))
	, _mouse(MouseClient(MouseServer::GetInstance()))
	, _keyboard(KeyboardClient(KeyboardServer::GetInstance())) { }


void GameState::Initialize() {
	// Create all Systems needed by the GameState ECS (keep ptr variables for quick access)
	_renderDefault = std::make_shared<RenderSystemDefault>(_ecs);
	_renderDefaultNoTex = std::make_shared<RenderSystemDefaultNoTex>(_ecs);
	_transformationSystem = std::make_shared<TransformationSystem>(_ecs);

	// Add all the GameState Systems to the ECS
	_ecs.AddSystem<RenderSystemDefault>(_renderDefault);
	_ecs.AddSystem<RenderSystemDefaultNoTex>(_renderDefaultNoTex);
	_ecs.AddSystem<TransformationSystem>(_transformationSystem);
}


bool GameState::HandleMessages() {
	return true;
}


void GameState::HandleInputs() {
	// Handle Input
	if (_keyboard.IsKeyPressed(GLFW_KEY_A)) { _camera.ChangePosition(StoneCold::Engine::MoveDirection::LEFT, 0.02f); }
	if (_keyboard.IsKeyPressed(GLFW_KEY_D)) { _camera.ChangePosition(StoneCold::Engine::MoveDirection::RIGHT, 0.02f); }
	if (_keyboard.IsKeyPressed(GLFW_KEY_W)) { _camera.ChangePosition(StoneCold::Engine::MoveDirection::FORWARD, 0.02f); }
	if (_keyboard.IsKeyPressed(GLFW_KEY_S)) { _camera.ChangePosition(StoneCold::Engine::MoveDirection::BACKWARD, 0.02f); }
	if (_keyboard.IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) { _camera.ChangePosition(StoneCold::Engine::MoveDirection::DOWN, 0.02f); }
	if (_keyboard.IsKeyPressed(GLFW_KEY_SPACE)) { _camera.ChangePosition(StoneCold::Engine::MoveDirection::UP, 0.02f); }
	_camera.ChangeLineOfSight((int)_mouse.GetMouseX(), (int)_mouse.GetMouseY(), true);

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
	_transformationSystem->Update(frameTime);

	//// Center the camera over the Player
	//auto& t = _ecs.GetComponentArray<TransformationComponent>()->at(_player);
	//_camera.x = (t.Position.X + ((t.Dimension.X * t.Scale) / 2.f)) - (WINDOW_WIDTH / 2.f);
	//_camera.y = (t.Position.Y + ((t.Dimension.Y * t.Scale) / 2.f)) - (WINDOW_HEIGHT / 2.f);
}


void GameState::Render() {
	_renderDefaultNoTex->Render(_projection, _camera.GetViewMatrix());
	_renderDefault->Render(_projection, _camera.GetViewMatrix());
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
