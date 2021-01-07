
#ifndef STONECOLD_SYSTEM_H
#define STONECOLD_SYSTEM_H

#include <vector>
#include <algorithm>
#include <iterator>
#include <glm/mat4x4.hpp>
#include "Types.hpp"

namespace StoneCold::Engine {

using namespace StoneCold::Base;

//
// System Interface
//
class System {
public:
	System(bitMask64 componentMask) : _componentMask(componentMask) { }
	System(const System&) = delete;
	System& operator=(const System&) = delete;

	virtual void HandleInputEvent(const std::vector<uint8>& keyStates) { }
	virtual void Update(uint64 frameTime) { }
	virtual void Render(const glm::mat4& projection, const glm::mat4& view) { }

	inline bitMask64 GetSystemMask() const { return _componentMask; }
	inline size_t GetEntitiesSize() const { return _entities.size(); }

	inline void AddEntity(entityId entityId) { if (std::find(_entities.begin(), _entities.end(), entityId) == _entities.end()) _entities.push_back(entityId); }
	inline void RemoveEntity(entityId entityId) { _entities.erase(std::remove(_entities.begin(), _entities.end(), entityId), _entities.end()); }

	virtual ~System() {}

protected:
	const bitMask64 _componentMask;
	std::vector<entityId> _entities;
};

}

#endif
