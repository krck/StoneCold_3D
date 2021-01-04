
#include "GameState.hpp"

using namespace StoneCold;
using namespace StoneCold::Engine;

GameState::GameState(uint16 maxEntities, SDL_Renderer* renderer, EngineCore* engine)
	: State(maxEntities, renderer, engine)
	, _eventManager(EventManager::GetInstance())
	, _mapTiles(std::vector<entityId>())
	, _animationSystem(nullptr)
	, _transformationSystem(nullptr)
	, _collisionDetectionSystem(nullptr)
	, _collisionResolutionSystem(nullptr)
	, _screenPositionSystem(nullptr)
	, _staticRenderSystem(nullptr)
	, _motionRenderSystem(nullptr)
	, _player(0)
	, _camera({ 0.f, 0.f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT }) { }


void GameState::Initialize() {
	// Create all Systems needed by the GameState ECS (keep ptr variables for quick access)
	_animationSystem = std::make_shared<AnimationSystem>(_ecs);
	_transformationSystem = std::make_shared<TransformationSystem>(_ecs);
	_collisionDetectionSystem = std::make_shared<CollisionDetectionSystem>(_ecs);
	_collisionResolutionSystem = std::make_shared<CollisionResolutionSystem>(_ecs);
	_screenPositionSystem = std::make_shared<ScreenPositionSystem>(_ecs);
	_staticRenderSystem = std::make_shared<StaticRenderSystem>(_renderer, _ecs);
	_motionRenderSystem = std::make_shared<MotionRenderSystem>(_renderer, _ecs);

	// Add all the GameState Systems to the ECS
	_ecs.AddSystem<AnimationSystem>(_animationSystem);
	_ecs.AddSystem<TransformationSystem>(_transformationSystem);
	_ecs.AddSystem<CollisionDetectionSystem>(_collisionDetectionSystem);
	_ecs.AddSystem<CollisionResolutionSystem>(_collisionResolutionSystem);
	_ecs.AddSystem<ScreenPositionSystem>(_screenPositionSystem);
	_ecs.AddSystem<StaticRenderSystem>(_staticRenderSystem);
	_ecs.AddSystem<MotionRenderSystem>(_motionRenderSystem);
}


bool GameState::HandleSDLEvent(const SDL_Event& sdlEvent) {
	// Check if any key was pressed ...
	if (sdlEvent.type == SDL_KEYDOWN) {
		// In case F5 was pressed: Trigger Level-Refresh
		if (sdlEvent.key.keysym.sym == SDLK_F5) {
			_eventManager.PublishEvent(EventCode::ChangeLevel);
		}
		// ESC to go back (to the Menu)
		else if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
			_engine->PopState();
		}

		return true;
	}
	return false;
}


void GameState::HandleInputEvent(const std::vector<uint8>& keyStates) {
	// Get the player components
	auto& a = _ecs.GetComponentArray<AnimationComponent>()->at(_player);
	auto& t = _ecs.GetComponentArray<TransformationComponent>()->at(_player);
	auto& v = _ecs.GetComponentArray<VelocityComponent>()->at(_player);

	// For each keykeyStates contains a value of 1 if pressed and a value of 0 if not pressed
	// Add negative and positive velocity so the sprite doesn't move if both are pressed at the same time
	v.Velocity.Y = (-1.0f * keyStates[SDL_SCANCODE_W]) + keyStates[SDL_SCANCODE_S];
	v.Velocity.X = (-1.0f * keyStates[SDL_SCANCODE_A]) + keyStates[SDL_SCANCODE_D];

	// Debug "dash"
	t.Speed = (keyStates[SDL_SCANCODE_RCTRL] ? t.BaseSpeed * 3 : t.BaseSpeed);

	// Update/Play a different Animation, based on the current input
	if (t.Speed > t.BaseSpeed)
		// Dash/Dodge movement
		a.CurrentAnimation = a.GetAnimation("jump");
	else if (v.Velocity.Y != 0.f || v.Velocity.X != 0.f)
		// Normal movement
		a.CurrentAnimation = a.GetAnimation("walk");
	else
		// No movement
		a.CurrentAnimation = a.GetAnimation("idle");
}


void GameState::Update(uint32 frameTime) {
	// Animate and Move all objects
	_animationSystem->Update(frameTime);
	_transformationSystem->Update(frameTime);
	// Now check for possible collisions
	_collisionDetectionSystem->Update(frameTime);
	_collisionResolutionSystem->Update(frameTime);
	// Finally set the Position where objects should be rendered
	_screenPositionSystem->Update(frameTime);

	// Center the camera over the Player
	auto& t = _ecs.GetComponentArray<TransformationComponent>()->at(_player);
	_camera.x = (t.Position.X + ((t.Dimension.X * t.Scale) / 2.f)) - (WINDOW_WIDTH / 2.f);
	_camera.y = (t.Position.Y + ((t.Dimension.Y * t.Scale) / 2.f)) - (WINDOW_HEIGHT / 2.f);
}


void GameState::Render() {
	// First: Render all static sprites (MapTiles)
	_staticRenderSystem->Render(_camera);
	// Second: Render all moving sprites (Player, NPCs, ...)
	_motionRenderSystem->Render(_camera);
	// Third: Render the GUI (always top Layer)
	// ...
}


void GameState::SetSpawnPosition(Vec2i spawnPoint) {
	// Update the players spawn point
	auto& t = _ecs.GetComponentArray<TransformationComponent>()->at(_player);
	t.Position.X = static_cast<float>(spawnPoint.X);
	t.Position.Y = static_cast<float>(spawnPoint.Y);
}


void GameState::SetEntities(entityId player, const std::vector<entityId>& mapTiles) {
	_player = player;
	_mapTiles = mapTiles;
}
