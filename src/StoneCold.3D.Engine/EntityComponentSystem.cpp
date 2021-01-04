
#include "EntityComponentSystem.hpp"

using namespace StoneCold::Engine;

EntityComponentSystem::EntityComponentSystem(uint16 maxEntities)
	: _availableEntities(std::queue<entityId>())
	, _entityComponents(std::vector<bitMask64>(maxEntities)) // Set vector size (like fixed array)
	, _world(EntityComponentWorld(maxEntities))
	, _systems(std::unordered_map<hash, std::shared_ptr<System>>()) {
	// Initialize the queue with all possible entity IDs
	for (entityId entityId = 0; entityId < maxEntities; ++entityId)
		_availableEntities.push(entityId);
}


entityId EntityComponentSystem::CreateEntity() {
	if (_availableEntities.size() == 0)
		throw std::out_of_range("No more Entity Ids available");

	// Get the first available ID of the queue
	entityId id = _availableEntities.front();
	_availableEntities.pop();
	// Reset the new Entities ComponentMask
	_entityComponents[id] = 0;
	return id;
}


void EntityComponentSystem::DestroyEntity(entityId entityId) {
	_world.DestroyEntity(entityId);
	// Invalidate the destroyed entity's ComponentMask
	// Put the destroyed ID at the back of the queue
	_entityComponents[entityId] = 0;
	_availableEntities.push(entityId);
}


//
// Workaround to set a additional Entity mask without a specific Component 
// (adding a Component always means adding a full Array of n Components)
//
void EntityComponentSystem::AddAdditionalSystemMask(entityId entityId, bitMask64 additionalMask) {
	_entityComponents[entityId] |= additionalMask;
	UpdateSystems(entityId, _entityComponents[entityId]);
}


void EntityComponentSystem::UpdateSystems(entityId entityId, bitMask64 entityComponentMask) {
	if (_systems.empty())
		throw std::out_of_range("Register at least one System before adding or removing Components");

	// Notify each system that an Entity's ComponentMask changed
	for (auto const& pair : _systems) {
		auto const& system = pair.second;
		auto const& systemMask = system->GetSystemMask();
		// Check if the Entity's ComponentMask matches the System and add (or remove)
		if ((entityComponentMask & systemMask) == systemMask)
			system->AddEntity(entityId);
		else
			system->RemoveEntity(entityId);
	}
}
