
#include "IntroState.hpp"

using namespace StoneCold;
using namespace StoneCold::Engine;

IntroState::IntroState(uint16 maxEntities, SDL_Renderer* renderer, EngineCore* engine)
	: State(maxEntities, renderer, engine)
	, _staticRenderSystem(nullptr)
	, _camera({ 0.f, 0.f, static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT) }) { }


void IntroState::Initialize() {
	// Create all Systems needed by the GameState ECS (keep ptr variables for quick access)
	_staticRenderSystem = std::make_shared<StaticRenderSystem>(_renderer, _ecs);

	// Add all the GameState Systems to the ECS
	_ecs.AddSystem<StaticRenderSystem>(_staticRenderSystem);
}


bool IntroState::HandleSDLEvent(const SDL_Event& sdlEvent) {
	// Check if any key was pressed ...
	if (sdlEvent.type == SDL_KEYDOWN) {
		if (_engine->HasState<MenuState>()) {
			auto menuPtr = _engine->GetState<MenuState>();
			_engine->PushState(menuPtr);
			return true;
		}
	}

	return false;
}


void IntroState::Render() {
	_staticRenderSystem->Render(_camera);
}
