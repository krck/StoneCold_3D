
#ifndef STONECOLD_ENTITYCOMPONENTSYSTEM_H
#define STONECOLD_ENTITYCOMPONENTSYSTEM_H

#include "Settings.hpp"
#include "System.hpp"
#include "EntityComponentWorld.hpp"
#include <array>
#include <queue>

namespace StoneCold::Engine {

	using namespace StoneCold::Base;

	//
	// Entity-Component-System (Manager)
	// based on: https://austinmorlan.com/posts/entitycomponent_system/
	//
	// Managing the creation and destruction of Entities. This includes distributing
	// Entity IDs and keeping record of which IDs are in use and which are not.
	//
	class EntityComponentSystem {
	public:
		EntityComponentSystem(uint16 maxEntities);

		EntityComponentSystem(const EntityComponentSystem&) = delete;
		EntityComponentSystem& operator=(const EntityComponentSystem&) = delete;

		entityId CreateEntity();
		void DestroyEntity(entityId entityId);

		template<typename T>
		void AddComponent(entityId entityId, T component) {
			// Add the Entity (and the component values) to the specific Component Array
			GetComponentArray<T>()->insert(entityId, component);
			// Add the Component Flag to the mask (OR bitmask with the current value)
			_entityComponents[entityId] |= ComponentMasks[GetTypeHash<T>()];
			UpdateSystems(entityId, _entityComponents[entityId]);
		}

		template<typename T>
		void RemoveComponent(entityId entityId) {
			// Remove the Entitiy (and its component values) from a specific Component Array
			GetComponentArray<T>()->erase(entityId);
			// Remove the Component Flag from the mask (AND with the inverse of the bitmask)
			_entityComponents[entityId] &= ~ComponentMasks[GetTypeHash<T>()];
			UpdateSystems(entityId, _entityComponents[entityId]);
		}

		template<typename T>
		void AddSystem(std::shared_ptr<System> system) {
			_systems.insert({ GetTypeHash<T>(), system });
		}

		//
		// Workaround to set a additional Entity mask without a specific Component 
		// (adding a Component always means adding a full Array of n Components)
		//
		void AddAdditionalSystemMask(entityId entityId, bitMask64 additionalMask);

		//
		// Get the statically casted pointer to the ComponentArray of type T
		//
		template<typename T>
		inline EntityComponentArray<T>* GetComponentArray() { return _world.GetComponentArray<T>(); }

		template<typename T>
		inline T* GetSystem() {
			return (_systems.find(GetTypeHash<T>()) != _systems.end())
				? static_cast<T*>(_systems[GetTypeHash<T>()].get())
				: nullptr;
		}

		inline size_t AvailableEntitiesSize() const { return _availableEntities.size(); }

		~EntityComponentSystem() = default;

	private:
		void UpdateSystems(entityId entityId, bitMask64 entityComponentMask);

	private:
		// Entity variables:
		std::queue<entityId> _availableEntities;
		std::vector<bitMask64> _entityComponents;
		// Component variables:
		EntityComponentWorld _world;
		// System variables:
		std::unordered_map<hash, std::shared_ptr<System>> _systems;
	};

}

#endif
