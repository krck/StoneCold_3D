
#include "EngineCore.hpp"

using namespace StoneCold;
using namespace StoneCold::Engine;


EngineCore::EngineCore()
	: _stateStack(std::stack<State*>())
	, _states(std::unordered_map<std::type_index, std::shared_ptr<State>>()) { };


bool EngineCore::Initialize(WindowManager* window) {
	if (window != nullptr) {
		_window = window;
		return true;
	}
	return false;
}


bool EngineCore::HandleMessages() {
	// Trigger Message handling of the current State / Scene and retrun if they handled it
	return _stateStack.top()->HandleMessages();
}


void EngineCore::HandleInputs() {
	// Trigger Input (Keyboard/Mouse) handling of the current State / Scene
	_stateStack.top()->HandleInputs();
}


void EngineCore::Update(uint64 frameTime) {
	// Update the current State / Scene
	_stateStack.top()->Update(frameTime);
}


void EngineCore::Render() {
	// Clear the Frame (white)
	_window->Clear();

	// Render the current State / Scene
	_stateStack.top()->Render();

	// Update Display-Screen and get Events
	_window->Update();
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
