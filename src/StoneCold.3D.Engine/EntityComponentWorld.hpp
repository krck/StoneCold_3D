
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
	static const bitMask64 RENDER_MOTION = 0x4000000000000000;
	static const bitMask64 RENDER_STATIC = 0x8000000000000000;

	//
	// Component (Bit-)Masks
	//
	static auto ComponentMasks = std::unordered_map<hash, const bitMask64>({
		{ GetTypeHash<AnimationComponent>(),				0x0000000000000001 },
		{ GetTypeHash<AttributeComponentUI>(),				0x0000000000000002 },
		{ GetTypeHash<CollisionComponent>(),				0x0000000000000004 },
		{ GetTypeHash<SpriteComponent>(),					0x0000000000000008 },
		{ GetTypeHash<SpriteLayeredComponent>(),			0x0000000000000010 },
		{ GetTypeHash<TransformationComponent>(),			0x0000000000000020 },
		{ GetTypeHash<VelocityComponent>(),					0x0000000000000040 },
		{ GetTypeHash<ScreenPositionComponent>(),			0x0000000000000080 },
		{ GetTypeHash<ScreenPositionLayeredComponent>(),	0x0000000000000100 },
		{ GetTypeHash<EnemyStateComponent>(),				0x0000000000000200 }
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

			_world.insert({ GetTypeHash<AnimationComponent>(), std::make_shared<EntityComponentArray<AnimationComponent>>(maxEntities) });
			_world.insert({ GetTypeHash<AttributeComponentUI>(), std::make_shared<EntityComponentArray<AttributeComponentUI>>(maxEntities) });
			_world.insert({ GetTypeHash<CollisionComponent>(), std::make_shared<EntityComponentArray<CollisionComponent>>(maxEntities) });
			_world.insert({ GetTypeHash<SpriteComponent>(), std::make_shared<EntityComponentArray<SpriteComponent>>(maxEntities) });
			_world.insert({ GetTypeHash<SpriteLayeredComponent>(), std::make_shared<EntityComponentArray<SpriteLayeredComponent>>(maxEntities) });
			_world.insert({ GetTypeHash<TransformationComponent>(), std::make_shared<EntityComponentArray<TransformationComponent>>(maxEntities) });
			_world.insert({ GetTypeHash<VelocityComponent>(), std::make_shared<EntityComponentArray<VelocityComponent>>(maxEntities) });
			_world.insert({ GetTypeHash<ScreenPositionComponent>(), std::make_shared<EntityComponentArray<ScreenPositionComponent>>(maxEntities) });
			_world.insert({ GetTypeHash<ScreenPositionLayeredComponent>(), std::make_shared<EntityComponentArray<ScreenPositionLayeredComponent>>(maxEntities) });
			_world.insert({ GetTypeHash<EnemyStateComponent>(), std::make_shared<EntityComponentArray<EnemyStateComponent>>(maxEntities) });
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
