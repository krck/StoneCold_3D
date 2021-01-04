
#ifndef STONECOLD_STATE_H
#define STONECOLD_STATE_H

#include "Types.hpp"
#include "SDL_Base.hpp"
#include "EntityComponentSystem.hpp"
#include <vector>

namespace StoneCold::Engine {

using namespace StoneCold::Base;

class EngineCore;

//
// State Interface
//
// There are more elegant ways of creating a Game-State-System, but here the State needs the 
// Engine (which is basically the StateManager) and the Engine needs the State ... obviously. 
//
// But this way State creation and destruction can be in one central
// place. Otherwise States have to create and cleanup each other.
//
class State {
public:
	State(uint16 maxEntities, SDL_Renderer* renderer, EngineCore* engine)
		: _ecs(EntityComponentSystem(maxEntities))
		, _renderer(renderer)
		, _engine(engine) { }

	inline EntityComponentSystem* GetECS() { return &_ecs; }

	virtual void Initialize() = 0;

	virtual void Start() { } // = 0;
	virtual void Stop() { } // = 0;

	virtual bool HandleSDLEvent(const SDL_Event& sdlEvent) = 0;
	virtual void HandleInputEvent(const std::vector<uint8>& keyStates) = 0;
	virtual void Update(uint32 frameTime) = 0;
	virtual void Render() = 0;

protected:
	EntityComponentSystem _ecs;
	SDL_Renderer* _renderer;
	EngineCore* _engine;
};

}

#endif