
#ifndef STONECOLD_GAMECORE_H
#define STONECOLD_GAMECORE_H

#include <array>
#include <chrono>
#include <string>
#include <iostream>
#include "WindowManager.hpp"
//#include "AiManager.hpp"
//#include "SDLManager.hpp"
//#include "EngineCore.hpp"
//#include "MapManager.hpp"
//#include "EventManager.hpp"
//#include "ResourceManager.hpp"
//#include "SimulationManager.hpp"
//#include <array>

namespace StoneCold::Game {

	using namespace std::chrono;
	//using namespace StoneCold::Engine;
	//using namespace StoneCold::Resources;

	//
	// This is the central Game class running the main-loop
	//
	class GameCore {
	public:
		GameCore();
		GameCore(const GameCore&) = delete;
		GameCore& operator=(const GameCore&) = delete;

		//
		// Setup SDL, create all SDL Resources and show the Window
		// Also initilaizes the Engine and the Managers in order
		//
		bool Initialize(const std::string& windowName);

		//
		// Run the games main-loop, handle the timer and FPS limiter 
		// and trigger the Engines Event, Update and Render functions
		//
		int Run();

		inline uint64 GetTicks() const { return (uint64)duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count(); }

		~GameCore() = default;

	private:
		InputManager _inputManager;
		WindowManager _windowManager;
		//EngineCore _engine;
		//ResourceManager _resources;
		//SimulationManager _simulation;
		//EventManager& _eventManager;
		//MapManager _mapManager;
		//AiManager _aiManager;
	};

}

#endif
