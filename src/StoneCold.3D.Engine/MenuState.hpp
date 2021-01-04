
#ifndef STONECOLD_MENUSTATE_H
#define STONECOLD_MENUSTATE_H

#include "EngineCore.hpp"
#include "GameState.hpp"
#include "EventManager.hpp"
#include "AnimationSystem.hpp"
#include "StaticRenderSystem.hpp"
#include "LayeredRenderSystem.hpp"
#include <algorithm>

namespace StoneCold::Engine {

class MenuState : public State {
public:
	MenuState(uint16 maxEntities, SDL_Renderer* renderer, EngineCore* engine);
	MenuState(const MenuState&) = delete;
	MenuState& operator=(const MenuState&) = delete;

	virtual void Initialize() override;

	virtual bool HandleSDLEvent(const SDL_Event& sdlEvent) override;
	virtual void HandleInputEvent(const std::vector<uint8>& keyStates) override { }
	virtual void Update(uint32 frameTime) override { }
	virtual void Render() override;

	void SetButtons(const std::vector<entityId>& buttons);

	~MenuState() = default;

private:
	void SetHoverButton(entityId hoverButton);
	void OnButtonPressPlay();
	void OnButtonPressOptions();
	void OnButtonPressCredits();
	void OnButtonPressQuit();

private:
	SDL_FRect _camera;
	EventManager& _eventManager;
	// EntityId's for fast access
	uint8 _selectedButtonIndex;
	std::vector<entityId> _buttons;
	// System ptrs for fast access
	std::shared_ptr<AnimationSystem> _animationSystem;
	std::shared_ptr<StaticRenderSystem> _staticRenderSystem;
	std::shared_ptr<LayeredRenderSystem> _layeredRenderSystem;
};

}

#endif
