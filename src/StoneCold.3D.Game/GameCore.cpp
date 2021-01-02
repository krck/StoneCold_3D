#include "GameCore.hpp"

using namespace StoneCold::Game;
//using namespace StoneCold::Resources;


GameCore::GameCore()
	: _inputManager(InputManager())
	, _windowManager(WindowManager())
{ };


bool GameCore::Initialize(const std::string& windowName) {
	try {
		if (_windowManager.Initialize(&_inputManager)) {
			// Get the SDL_Renderer and setup the Engine
			_windowManager.SetupWindow("StoneCold 3D", WINDOW_WIDTH, WINDOW_HEIGHT, false);

			// Setup all the additional Managers in the correct order
			// ...

			// Setup the randomizer with a seed
			std::srand(RNG_SEED);

			//// Load all global Resources and create the basic States
			//_simulation.CreateIntroState();
			//_simulation.CreateGameState();
			//_simulation.CreateMenuState();
			//// Load a first Level and add it to the GameState
			//_simulation.LoadLevel();

			//// Push the first State to update and render
			//auto firstState = _engine.GetState<IntroState>();
			//_engine.PushState(firstState);

			return true;
		}
		else {
			return false;
		}
	}
	catch (const std::exception & ex) {
		std::cout << ex.what() << std::endl;
		return false;
	}
}


//
// Run the main Game-loop
//
int GameCore::Run() {
	try {
		const MouseClient mouse = MouseClient(MouseServer::GetInstance());
		const KeyboardClient keyboard = KeyboardClient(KeyboardServer::GetInstance());

		// Loop timer variables
		uint64 timeStamp_new = GetTicks();
		uint64 timeStamp_old = GetTicks();
		uint64 frameTime = 0; // delta in ms
		// FPS Counter variables
		const uint8 frameTimeSize = 20;
		auto frameTimes = std::array<uint64, frameTimeSize>();
		uint64 frameCount = 0;
		float averageFPS = 0.f;

		// Start the main loop
		while (!_windowManager.IsClosed())
		{
			timeStamp_new = GetTicks();
			frameTime = timeStamp_new - timeStamp_old;

			_windowManager.Clear();

			// Handle Input
			if (keyboard.IsKeyPressed(GLFW_KEY_A)) { std::cout << "A" << std::endl; }
			if (keyboard.IsKeyPressed(GLFW_KEY_D)) { std::cout << "D" << std::endl; }
			if (keyboard.IsKeyPressed(GLFW_KEY_W)) { std::cout << "W" << std::endl; }
			if (keyboard.IsKeyPressed(GLFW_KEY_S)) { std::cout << "S" << std::endl; }

			// Update Display-Screen and get Events
			_windowManager.Update();

			// FPS counter (average)
			frameTimes[frameCount] = frameTime;
			frameCount++;
			if (frameCount == frameTimeSize) {
				frameCount = 0;
				averageFPS = 0.f;
				for (uint8 i = 0; i < frameTimeSize; i++) { averageFPS += frameTimes[i]; }
				averageFPS = 1000.f / (averageFPS / frameTimeSize);
				std::cout << "FPS: " << (uint16)averageFPS << "\n";
			}
			timeStamp_old = timeStamp_new;
		}

		return 0;
	}
	catch (const std::exception & ex) {
		std::cout << ex.what() << std::endl;
		return -1;
	}
}
