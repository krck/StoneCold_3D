
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
	IntroState(uint16 maxEntities, SDL_Renderer* renderer, EngineCore* engine);
	IntroState(const IntroState&) = delete;
	IntroState& operator=(const IntroState&) = delete;

	virtual void Initialize() override;

	virtual bool HandleSDLEvent(const SDL_Event& sdlEvent) override;
	virtual void HandleInputEvent(const std::vector<uint8>& keyStates) override { }
	virtual void Update(uint32 frameTime) override { /* Nothing to update. Scene is static */ }
	virtual void Render() override;

	~IntroState() = default;

private:
	SDL_FRect _camera;	
	// System ptrs for fast access
	std::shared_ptr<StaticRenderSystem> _staticRenderSystem;

};

}

#endif
