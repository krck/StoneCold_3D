
#include "MenuState.hpp"

using namespace StoneCold;
using namespace StoneCold::Engine;

MenuState::MenuState(uint16 maxEntities, SDL_Renderer* renderer, EngineCore* engine)
	: State(maxEntities, renderer, engine)
	, _eventManager(EventManager::GetInstance())
	, _selectedButtonIndex(0)
	, _animationSystem(nullptr)
	, _staticRenderSystem(nullptr)
	, _layeredRenderSystem(nullptr)
	, _buttons(std::vector<entityId>())
	, _camera({ 0.f, 0.f, static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT) }) { }


void MenuState::Initialize() {
	// Create all Systems needed by the GameState ECS (keep ptr variables for quick access)
	_animationSystem = std::make_shared<AnimationSystem>(_ecs);
	_staticRenderSystem = std::make_shared<StaticRenderSystem>(_renderer, _ecs);
	_layeredRenderSystem = std::make_shared<LayeredRenderSystem>(_renderer, _ecs);

	// Add all the GameState Systems to the ECS
	_ecs.AddSystem<AnimationSystem>(_animationSystem);
	_ecs.AddSystem<StaticRenderSystem>(_staticRenderSystem);
	_ecs.AddSystem<LayeredRenderSystem>(_layeredRenderSystem);
}


bool MenuState::HandleSDLEvent(const SDL_Event& sdlEvent) {
	// Check if any key was pressed ...
	if (sdlEvent.type == SDL_KEYDOWN) {
		// Up/Down (W/S) to select the Buttons
		if (sdlEvent.key.keysym.sym == SDLK_DOWN || sdlEvent.key.keysym.sym == SDLK_s) {
			_selectedButtonIndex = (_selectedButtonIndex < (_buttons.size() - 1)) ? _selectedButtonIndex + 1 : 0;
			SetHoverButton(_buttons[_selectedButtonIndex]);
		}
		else if (sdlEvent.key.keysym.sym == SDLK_UP || sdlEvent.key.keysym.sym == SDLK_w) {
			_selectedButtonIndex = (_selectedButtonIndex > 0) ? _selectedButtonIndex - 1 : _buttons.size() - 1;
			SetHoverButton(_buttons[_selectedButtonIndex]);
		}
		// Enter to execute the Button function
		else if (sdlEvent.key.keysym.sym == SDLK_RETURN) {
			switch (_selectedButtonIndex)
			{
			case 0: OnButtonPressPlay(); break;
			case 1: OnButtonPressOptions(); break;
			case 2: OnButtonPressCredits(); break;
			case 3: OnButtonPressQuit(); break;
			default: break;
			}
		}
		// Backspace to go back (to the Intro)
		else if (sdlEvent.key.keysym.sym == SDLK_BACKSPACE) {
			_engine->PopState();
		}

		return true;
	}
	return false;
}


void MenuState::Render() {
	_staticRenderSystem->Render(_camera);
	_layeredRenderSystem->Render(_camera);
}


void MenuState::SetButtons(const std::vector<entityId>& buttons) {
	if (!buttons.empty()) {
		// Link all the Buttons to their functions
		_buttons = buttons;
		_selectedButtonIndex = 0;
		// Set the first Button to "hover"
		SetHoverButton(buttons[_selectedButtonIndex]);
	}
}


void MenuState::SetHoverButton(entityId hoverButton) {
	// No need for a specific System
	auto& position = *_ecs.GetComponentArray<ScreenPositionLayeredComponent>();
	auto& attributes = *_ecs.GetComponentArray<AttributeComponentUI>();
	auto& animations = *_ecs.GetComponentArray<AnimationComponent>();
	// Check all Buttons and set the selected one to "hover", all others to "idle"
	for (const auto& button : _buttons) {
		auto& ps = position[button];
		auto& at = attributes[button];
		auto& an = animations[button];
		if (button != hoverButton) {
			at.UiAttribute = UiElementAttribute::UIE_Idle;
			an.CurrentAnimation = &an.Animations->find("idle")->second;
			ps.SrcRectBottom = an.CurrentAnimation->FramePositions[an.CurrentFrameIndex];
		}
		else {
			at.UiAttribute = UiElementAttribute::UIE_Hover;
			an.CurrentAnimation = &an.Animations->find("hover")->second;
			ps.SrcRectBottom = an.CurrentAnimation->FramePositions[an.CurrentFrameIndex];
		}
	}
}


void MenuState::OnButtonPressPlay() {
	if (_engine->HasState<GameState>()) {
		auto gamePtr = _engine->GetState<GameState>();
		_engine->PushState(gamePtr);
	}
}

void MenuState::OnButtonPressOptions() {

}

void MenuState::OnButtonPressCredits() {

}

void MenuState::OnButtonPressQuit() {
	_eventManager.PublishEvent(EventCode::Quit);
}
