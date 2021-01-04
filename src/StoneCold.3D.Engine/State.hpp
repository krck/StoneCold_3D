
#ifndef STONECOLD_STATE_H
#define STONECOLD_STATE_H

#include "Types.hpp"
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
	State(uint16 maxEntities, EngineCore* engine)
		: _ecs(EntityComponentSystem(maxEntities))
		, _engine(engine) { }

	inline EntityComponentSystem* GetECS() { return &_ecs; }

	virtual void Initialize() = 0;

	virtual void Start() { } // = 0;
	virtual void Stop() { } // = 0;

	virtual bool HandleMessages() = 0;
	virtual void HandleInputs() = 0;
	virtual void Update(uint64 frameTime) = 0;
	virtual void Render() = 0;

protected:
	EntityComponentSystem _ecs;
	EngineCore* _engine;
};

}

#endif