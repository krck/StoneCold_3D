
#include "EngineCore.hpp"

using namespace StoneCold;
using namespace StoneCold::Engine;


EngineCore::EngineCore()
	: _renderer(nullptr)
	, _stateStack(std::stack<State*>())
	, _states(std::unordered_map<std::type_index, std::shared_ptr<State>>()) { };


bool EngineCore::Initialize(SDL_Renderer* renderer) {
	if (renderer != nullptr) {
		_renderer = renderer;
		return true;
	}
	else {
		return false;
	}
}


bool EngineCore::HandleSDLEvent(const SDL_Event& sdlEvent) {
	// Pass SDL Events to the current State / Scene and retrun if they handled it
	return _stateStack.top()->HandleSDLEvent(sdlEvent);
}


void EngineCore::HandleInputEvent(const std::vector<uint8>& keyStates) {
	// Pass Keyboard Events to the current State / Scene
	_stateStack.top()->HandleInputEvent(keyStates);
}


void EngineCore::Update(uint32 frameTime) {
	// Update the current State / Scene
	_stateStack.top()->Update(frameTime);
}


void EngineCore::Render() {
	// Clear the Frame (white)
	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
	SDL_RenderClear(_renderer);

	// Render the current State / Scene
	_stateStack.top()->Render();

	// Swap render buffer to the Window
	SDL_RenderPresent(_renderer);
}


void EngineCore::PushState(State* state) {
	// Stop the current State
	if (!_stateStack.empty()) {
		_stateStack.top()->Stop();
	}

	// Push and start the new State
	_stateStack.push(state);
	_stateStack.top()->Start();
}


void EngineCore::PopState() {
	// Stop and pop the current state
	if (!_stateStack.empty()) {
		_stateStack.top()->Stop();
		_stateStack.pop();
	}

	// Start previous state again
	if (!_stateStack.empty()) {
		_stateStack.top()->Start();
	}
}
