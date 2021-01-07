
#ifndef STONECOLD_MENUSTATE_H
#define STONECOLD_MENUSTATE_H

#include <algorithm>
#include "EngineCore.hpp"
#include "GameState.hpp"
#include "MessageService.hpp"

namespace StoneCold::Engine {

class MenuState : public State {
public:
	MenuState(uint16 maxEntities, EngineCore* engine);
	MenuState(const MenuState&) = delete;
	MenuState& operator=(const MenuState&) = delete;

	virtual void Initialize() override;

	virtual bool HandleMessages() override { return false; }
	virtual void HandleInputs() override;
	virtual void Update(uint64 frameTime) override { /* Nothing to update. Scene is static */ }
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
	KeyboardClient _keyboard;
	MessageService* _messageService;
	// EntityId's for fast access
	uint8 _selectedButtonIndex;
	std::vector<entityId> _buttons;
	// System ptrs for fast access
	//std::shared_ptr<AnimationSystem> _animationSystem;
	//std::shared_ptr<StaticRenderSystem> _staticRenderSystem;
	//std::shared_ptr<LayeredRenderSystem> _layeredRenderSystem;
};

}

#endif
