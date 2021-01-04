
#include "GameState.hpp"

using namespace StoneCold;
using namespace StoneCold::Engine;

GameState::GameState(uint16 maxEntities, EngineCore* engine)
	: State(maxEntities, engine)
	, _messageService(MessageService::GetInstance())
	, _mapTiles(std::vector<entityId>())
	, _animationSystem(nullptr)
	, _transformationSystem(nullptr)
	, _collisionDetectionSystem(nullptr)
	, _collisionResolutionSystem(nullptr)
	, _screenPositionSystem(nullptr)
	, _staticRenderSystem(nullptr)
	, _motionRenderSystem(nullptr)
	, _player(0)
	, _camera(Camera(glm::vec3(0.0f, 10.0f, 10.f)))
	, _mouse(MouseClient(MouseServer::GetInstance()))
	, _keyboard(KeyboardClient(KeyboardServer::GetInstance())) { }


void GameState::Initialize() {
	// Create all Systems needed by the GameState ECS (keep ptr variables for quick access)
	_animationSystem = std::make_shared<AnimationSystem>(_ecs);
	_transformationSystem = std::make_shared<TransformationSystem>(_ecs);
	_collisionDetectionSystem = std::make_shared<CollisionDetectionSystem>(_ecs);
	_collisionResolutionSystem = std::make_shared<CollisionResolutionSystem>(_ecs);
	_screenPositionSystem = std::make_shared<ScreenPositionSystem>(_ecs);
	_staticRenderSystem = std::make_shared<StaticRenderSystem>(_ecs);
	_motionRenderSystem = std::make_shared<MotionRenderSystem>(_ecs);

	// Add all the GameState Systems to the ECS
	_ecs.AddSystem<AnimationSystem>(_animationSystem);
	_ecs.AddSystem<TransformationSystem>(_transformationSystem);
	_ecs.AddSystem<CollisionDetectionSystem>(_collisionDetectionSystem);
	_ecs.AddSystem<CollisionResolutionSystem>(_collisionResolutionSystem);
	_ecs.AddSystem<ScreenPositionSystem>(_screenPositionSystem);
	_ecs.AddSystem<StaticRenderSystem>(_staticRenderSystem);
	_ecs.AddSystem<MotionRenderSystem>(_motionRenderSystem);
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
	// Animate and Move all objects
	_animationSystem->Update(frameTime);
	_transformationSystem->Update(frameTime);
	// Now check for possible collisions
	_collisionDetectionSystem->Update(frameTime);
	_collisionResolutionSystem->Update(frameTime);
	// Finally set the Position where objects should be rendered
	_screenPositionSystem->Update(frameTime);

	//// Center the camera over the Player
	//auto& t = _ecs.GetComponentArray<TransformationComponent>()->at(_player);
	//_camera.x = (t.Position.X + ((t.Dimension.X * t.Scale) / 2.f)) - (WINDOW_WIDTH / 2.f);
	//_camera.y = (t.Position.Y + ((t.Dimension.Y * t.Scale) / 2.f)) - (WINDOW_HEIGHT / 2.f);
}


void GameState::Render() {
	// First: Render all static sprites (MapTiles)
	_staticRenderSystem->Render();
	// Second: Render all moving sprites (Player, NPCs, ...)
	_motionRenderSystem->Render();
	// Third: Render the GUI (always top Layer)
	// ...
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
