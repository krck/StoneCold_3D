
#ifndef STONECOLD_ENGINECORE_H
#define STONECOLD_ENGINECORE_H

#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <memory>
#include <stack>
#include "State.hpp"
#include "Exception.hpp"
#include "MessageService.hpp"
#include "WindowManager.hpp"

namespace StoneCold::Engine {

	//
	// EngineCore (and StateManager)
	//
	// Directly called from the games main-loop, this holds and manages all GameStates,
	// distributes SDL_Events and Update calls and SDL_RenderPresent's to the screen
	//
	class EngineCore {
	public:
		EngineCore();
		EngineCore(const EngineCore&) = delete;
		EngineCore& operator=(const EngineCore&) = delete;

		bool Initialize(WindowManager* window);

		bool HandleMessages();
		void HandleInputs();
		void Update(uint64 frameTime);
		void Render();

		void PushState(State* state);
		void PopState();

		//
		// Add a new State to the Entity based on the State Type
		// Each Entity can have any State but only one active instance of each Type
		// (type_index is a wrapper class around a std::type_info object, that can be used as index)
		//
		template<typename T>
		void AddState(std::shared_ptr<State>&& state) { _states[std::type_index(typeid(T))] = std::move(state); }

		template<typename T>
		T* GetState() { return static_cast<T*>(_states[std::type_index(typeid(T))].get()); }

		template<typename T>
		bool HasState() const { return (_states.find(std::type_index(typeid(T))) != _states.end()); }

		template<typename T>
		void RemoveState() {
			auto state = std::type_index(typeid(T));
			auto stateIter = _states.find(state);
			if (stateIter != _states.end()) {
				stateIter->second->Stop();
				_states.erase(state);
			}
		}

		~EngineCore() = default;

	private:
		//
		// State Management (as part of the core Engine)
		// Top of the stack is the active state thats updated and rendered
		//
		std::stack<State*> _stateStack;
		std::unordered_map<std::type_index, std::shared_ptr<State>> _states;
		WindowManager* _window;
	};

}

#endif
