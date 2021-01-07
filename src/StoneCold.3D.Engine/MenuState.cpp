
#include "MenuState.hpp"

using namespace StoneCold;
using namespace StoneCold::Engine;

MenuState::MenuState(uint16 maxEntities, EngineCore* engine)
	: State(maxEntities, engine)
	, _messageService(MessageService::GetInstance())
	, _selectedButtonIndex(0)
	//, _animationSystem(nullptr)
	//, _staticRenderSystem(nullptr)
	//, _layeredRenderSystem(nullptr)
	, _buttons(std::vector<entityId>())
	, _keyboard(KeyboardClient(KeyboardServer::GetInstance())) { }


void MenuState::Initialize() {
	// Create all Systems needed by the GameState ECS (keep ptr variables for quick access)
	//_animationSystem = std::make_shared<AnimationSystem>(_ecs);
	//_staticRenderSystem = std::make_shared<StaticRenderSystem>(_ecs);
	//_layeredRenderSystem = std::make_shared<LayeredRenderSystem>(_ecs);

	// Add all the GameState Systems to the ECS
	//_ecs.AddSystem<AnimationSystem>(_animationSystem);
	//_ecs.AddSystem<StaticRenderSystem>(_staticRenderSystem);
	//_ecs.AddSystem<LayeredRenderSystem>(_layeredRenderSystem);
}


void MenuState::HandleInputs() {
	// Up/Down (W/S) to select the Buttons
	if (_keyboard.IsKeyPressed(GLFW_KEY_DOWN) || _keyboard.IsKeyPressed(GLFW_KEY_S)) {
		_selectedButtonIndex = (_selectedButtonIndex < (_buttons.size() - 1)) ? _selectedButtonIndex + 1 : 0;
		SetHoverButton(_buttons[_selectedButtonIndex]);
	}
	else if (_keyboard.IsKeyPressed(GLFW_KEY_UP) || _keyboard.IsKeyPressed(GLFW_KEY_W)) {
		_selectedButtonIndex = (_selectedButtonIndex > 0) ? _selectedButtonIndex - 1 : (uint8)_buttons.size() - 1;
		SetHoverButton(_buttons[_selectedButtonIndex]);
	}
	// Enter to execute the Button function
	else if (_keyboard.IsKeyPressed(GLFW_KEY_ENTER)) {
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
	else if (_keyboard.IsKeyPressed(GLFW_KEY_BACKSPACE)) {
		_engine->PopState();
	}
}


void MenuState::Render() {
	//_staticRenderSystem->Render();
	//_layeredRenderSystem->Render();
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
	//auto& position = *_ecs.GetComponentArray<ScreenPositionLayeredComponent>();
	//auto& attributes = *_ecs.GetComponentArray<AttributeComponentUI>();
	//auto& animations = *_ecs.GetComponentArray<AnimationComponent>();
	// Check all Buttons and set the selected one to "hover", all others to "idle"
	for (const auto& button : _buttons) {
		//auto& ps = position[button];
		//auto& at = attributes[button];
		//auto& an = animations[button];
		if (button != hoverButton) {
			//at.UiAttribute = UiElementAttribute::UIE_Idle;
			//an.CurrentAnimation = &an.Animations->find("idle")->second;
			//ps.SrcRectBottom = an.CurrentAnimation->FramePositions[an.CurrentFrameIndex];
		}
		else {
			//at.UiAttribute = UiElementAttribute::UIE_Hover;
			//an.CurrentAnimation = &an.Animations->find("hover")->second;
			//ps.SrcRectBottom = an.CurrentAnimation->FramePositions[an.CurrentFrameIndex];
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
	//_messageService.PublishEvent(EventCode::Quit);
}
