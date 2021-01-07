
#include "IntroState.hpp"

using namespace StoneCold;
using namespace StoneCold::Engine;

IntroState::IntroState(uint16 maxEntities, EngineCore* engine)
	: State(maxEntities, engine)
	//, _staticRenderSystem(nullptr)
	, _keyboard(KeyboardClient(KeyboardServer::GetInstance())) { }


void IntroState::Initialize() {
	// Create all Systems needed by the GameState ECS (keep ptr variables for quick access)
	//_staticRenderSystem = std::make_shared<StaticRenderSystem>(_ecs);

	// Add all the GameState Systems to the ECS
	//_ecs.AddSystem<StaticRenderSystem>(_staticRenderSystem);

	// Reset the Keyboard input buffer
	_keyboard.FlushKeyBuffer();
}


void IntroState::HandleInputs() {
	// Check if any key was pressed and switch to the main menu
	if (_keyboard.ReadKey() != 0) {
		auto menuPtr = _engine->GetState<MenuState>();
		_engine->PushState(menuPtr);
	}
}


void IntroState::Render() {
	//_staticRenderSystem->Render();
}
