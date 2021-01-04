
#ifndef STONECOLD_ENTITYCOMPONENTARRAY_H
#define STONECOLD_ENTITYCOMPONENTARRAY_H

#include "Types.hpp"
#include <stdexcept>
#include <unordered_map>
#include <algorithm>

namespace StoneCold::Engine {

using namespace StoneCold::Base;

//
// IEntityComponentArray
//
// Simple interface with only one purpose: If a Entity (ID) gets destroyed
// all EntityComponentArray<T>s must remove the Entities compoents. This is done
// in a loop by the EntityComponentManager and not possible with templated classes
//
class IEntityComponentArray {
public:
	virtual ~IEntityComponentArray() = default;
	virtual void DestroyEntity(entityId entityId) = 0;
};

//
// EntityComponentArray
// based on: https://austinmorlan.com/posts/entitycomponent_system/
//
// Array-like container thats always "packed" in memory
// and maps any Entity-ID to a corresponding array index.
//
// This way random access [] can be done directly with the Entities and iterating
// from 0 to .size() operates only on valid Components for great caching-results.
//
template <typename T>
class EntityComponentArray : public IEntityComponentArray {
public:
	EntityComponentArray(size_t ms) noexcept
		: _entityIndexMap(std::unordered_map<entityId, size_t>()), _maxSize(ms), _size(0), _data(new T[ms]) {
		// Always initialize the array with empty values
		for (size_t i = 0; i < _maxSize; i++)
			_data[i] = T();
	}

	inline T& operator[](entityId entityId) { return _data[_entityIndexMap[entityId]]; }
	inline const T& operator[](entityId entityId) const { return _data[_entityIndexMap[entityId]]; }

	T& at(entityId entityId) {
		if (_entityIndexMap.find(entityId) != _entityIndexMap.end())
			return _data[_entityIndexMap[entityId]];
		throw std::out_of_range("Index not available");
	}

	const T& at(entityId entityId) const {
		if (_entityIndexMap.find(entityId) != _entityIndexMap.end())
			return _data[_entityIndexMap[entityId]];
		throw std::out_of_range("Index not available");
	}

	inline constexpr size_t max_size() const noexcept { return _maxSize; }
	inline size_t size() const noexcept { return _size; }
	inline bool empty() const noexcept { return (_size == 0); }

	//
	// Add a new Component to the back of the Array. The Component can be accessed via
	// its Entity (and a Entity-Index pair will be created automatically, based on size())
	//
	// In case the Entity has a Component (and index) already, the value will be updated
	//
	bool insert(entityId entityId, T component) {
		if (_size >= _maxSize)
			return false;

		if (_entityIndexMap.find(entityId) == _entityIndexMap.end()) {
			_entityIndexMap[entityId] = _size;
			_data[_size] = component;
			_size++;
		}
		else {
			_data[_entityIndexMap[entityId]] = component;
		}
		return true;
	}

	//
	// Remove a Component from the Array. The Component data and Entity-Index pair will
	// be deleted and the last Component from the array will be swapped in its place, to
	// keep the "tightly packed" memory.
	//
	bool erase(entityId entityId) {
		if (_entityIndexMap.find(entityId) == _entityIndexMap.end())
			return false;

		// Get the last Entity-Index value, based on the last item in the _data array (_size - 1)
		auto lastEntity = std::find_if(_entityIndexMap.begin(), _entityIndexMap.end(), [this](const auto& p) { return p.second == (_size - 1); });
		auto currentIndex = _entityIndexMap[entityId];

		// Swap the last Component value with the one to erase (and clear the last)
		_data[_entityIndexMap[entityId]] = _data[_entityIndexMap[lastEntity->first]];
		_data[_entityIndexMap[lastEntity->first]] = T();
		_size--;

		// Swap the last Entities Index value with the one to erase (and clear from map)
		_entityIndexMap[lastEntity->first] = currentIndex;
		_entityIndexMap.erase(entityId);
		return true;
	}

	inline void DestroyEntity(entityId entityId) override { erase(entityId); }

	inline const T* GetRawData() const noexcept { return _data; }

	inline const std::unordered_map<entityId, size_t>& GetEntityIndexMap() const noexcept { return _entityIndexMap; }

	~EntityComponentArray() {
		delete[] _data;
		_entityIndexMap.clear();
		_maxSize = 0;
		_size = 0;
	}

private:
	std::unordered_map<entityId, size_t> _entityIndexMap;
	size_t _maxSize;
	size_t _size;
	T* _data;
};

}

#endif
