
#ifndef STONECOLD_INTROSTATE_H
#define STONECOLD_INTROSTATE_H

#include "EngineCore.hpp"
#include "MenuState.hpp"
#include "ScreenPositionSystem.hpp"
#include "StaticRenderSystem.hpp"
#include <algorithm>

namespace StoneCold::Engine {

class IntroState : public State {
public:
	IntroState(uint16 maxEntities, EngineCore* engine);
	IntroState(const IntroState&) = delete;
	IntroState& operator=(const IntroState&) = delete;

	virtual void Initialize() override;

	virtual bool HandleMessages() override { return false; }
	virtual void HandleInputs() override;
	virtual void Update(uint64 frameTime) override { /* Nothing to update. Scene is static */ }
	virtual void Render() override;

	~IntroState() = default;

private:
	KeyboardClient _keyboard;
	// System ptrs for fast access
	std::shared_ptr<StaticRenderSystem> _staticRenderSystem;

};

}

#endif
