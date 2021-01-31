
#ifndef STONECOLD_ENTITYCOMPONENTWORLD_H
#define STONECOLD_ENTITYCOMPONENTWORLD_H

#include "Components.hpp"
#include "EntityComponentArray.hpp"
#include <unordered_map>

namespace StoneCold::Engine {

	using namespace StoneCold::Base;

	//
	// System (Bit-)Masks
	//
	// Workaround to set a additional Entity mask without a specific Component 
	// (adding a Component always means adding a full Array of n Components)
	//
	static const bitMask64 MASK_SHADER_MAP =	            0x8000000000000000;
	static const bitMask64 MASK_SHADER_DEFAULT =		    0x4000000000000000;
    static const bitMask64 MASK_SHADER_INSTANCED =          0x2000000000000000;

	//
	// Component (Bit-)Masks
	//
	static auto ComponentMasks = std::unordered_map<hash, const bitMask64>({
		{ GetTypeHash<MeshComponent>(),						0x0000000000000001 },
		{ GetTypeHash<TextureComponent>(),					0x0000000000000002 },
		{ GetTypeHash<TransformationComponent>(),			0x0000000000000004 },
		{ GetTypeHash<VelocityComponent>(),					0x0000000000000008 },
		{ GetTypeHash<InstanceComponent>(),			        0x0000000000000010 }
		});

	template<typename T>
	inline const bitMask64 GetComponentMask() { return ComponentMasks[GetTypeHash<T>()]; }

	//
	// Entity-Component World
	//
	// Wrapper around all the Component Arrays to initialize them with a variable
	// size, so each State (Menu, Intro, Game) can have a "isolated" ECS instance
	//
	class EntityComponentWorld {
	public:
		EntityComponentWorld(uint16 maxEntities) : _maxEntities(maxEntities) {
			// Initialize the queue with all possible entity IDs
			_world = std::unordered_map<hash, std::shared_ptr<IEntityComponentArray>>();

			_world.insert({ GetTypeHash<MeshComponent>(), std::make_shared<EntityComponentArray<MeshComponent>>(maxEntities) });
			_world.insert({ GetTypeHash<TextureComponent>(), std::make_shared<EntityComponentArray<TextureComponent>>(maxEntities) });
			_world.insert({ GetTypeHash<TransformationComponent>(), std::make_shared<EntityComponentArray<TransformationComponent>>(maxEntities) });
			_world.insert({ GetTypeHash<VelocityComponent>(), std::make_shared<EntityComponentArray<VelocityComponent>>(maxEntities) });
            _world.insert({ GetTypeHash<InstanceComponent>(), std::make_shared<EntityComponentArray<InstanceComponent>>(maxEntities) });
		}

		EntityComponentWorld(const EntityComponentWorld&) = delete;
		EntityComponentWorld& operator=(const EntityComponentWorld&) = delete;

		inline uint16 GetWorldSize() const { return _maxEntities; }

		template<typename T>
		inline EntityComponentArray<T>* GetComponentArray() {
			return dynamic_cast<EntityComponentArray<T>*>(_world[GetTypeHash<T>()].get());
		}

		//
		// Notify each component array that an entity has been destroyed
		// If it has a component for that entity, it will remove it
		//
		void DestroyEntity(entityId entityId) {
			for (auto const& pair : _world) {
				auto const& component = pair.second;
				component->DestroyEntity(entityId);
			}
		}

	private:
		const uint16 _maxEntities;
		std::unordered_map<hash, std::shared_ptr<IEntityComponentArray>> _world;
	};

}

#endif
