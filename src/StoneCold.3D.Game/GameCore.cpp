#include "GameCore.hpp"

using namespace StoneCold::Game;
using namespace StoneCold::Engine;


GameCore::GameCore()
	: _inputManager(InputManager())
	, _windowManager(WindowManager())
	, _engine(EngineCore())
	, _loader(LoadManager())
	, _resources(ResourceManager())
	, _simulation(SimulationManager())
	, _mapManager(MapManager())
{ };


//
// Setup and initialize all Managers in the correct order
//
bool GameCore::Initialize(const std::string& windowName) {
	try {
		_inputManager.Initialize(KeyboardServer::GetInstance(), MouseServer::GetInstance());
		if (_windowManager.Initialize(&_inputManager)) {
			bool allGood = true;

			// Create the actual OpenGL Context Window and init all of the core systems
			allGood &= _windowManager.SetupWindow("StoneCold 3D", WINDOW_WIDTH, WINDOW_HEIGHT, false);
			allGood &= _engine.Initialize(&_windowManager);
			allGood &= _resources.Initialize(&_loader);
			allGood &= _simulation.Initialize(&_engine, &_resources, &_mapManager);

			if (!allGood)
				throw GameException("Failed to initialize the Game");

			// Setup the randomizer with a seed
			std::srand(RNG_SEED);

			// Load all global Resources and create the basic States
			_simulation.CreateIntroState();
			_simulation.CreateGameState();
			_simulation.CreateMenuState();
			// Load a first Level and add it to the GameState
			_simulation.LoadLevel();

			// Push the first State to update and render
			auto firstState = _engine.GetState<GameState>();
			_engine.PushState(firstState);

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
		// Loop timer variables
		uint64 timeStamp_new = GetTicks();
		uint64 timeStamp_old = GetTicks();
		uint64 frameTime = 0; // delta in ms
		// FPS Counter variables
		const uint8 frameTimeSize = FRAME_SAMPLES;
		auto frameTimes = std::array<uint64, FRAME_SAMPLES>();
		uint64 frameCount = 0;
		float averageFPS = 0.f;







		//auto model = _loader.LoadSimpleMeshFromFile(basePath + "models\\coordinates\\coordinates.obj");

		//// Create a full Shader program with Vertex Shader and Fragment Shader
		//auto shaderDefault = std::make_unique<StoneCold::Resources::DefaultShaderNoTex>();

		//glm::mat4 projection = glm::perspective(45.0f, (_windowManager.GetWidth() / (float)_windowManager.GetHeight()), 0.1f, 1000.0f);

		//glm::mat4 transform = glm::mat4(1.0f);
		////transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -3.0f));
		//transform = glm::scale(transform, glm::vec3(1.0f));

		//glm::vec3 rotation = glm::vec3(0.0f, 1.0f, 0.0f);






		// Start the main loop
		while (!_windowManager.IsClosed())
		{
			timeStamp_new = GetTicks();
			frameTime = timeStamp_new - timeStamp_old;

			// Engine core functions
			_engine.HandleMessages();
			_engine.HandleInputs();
			_engine.Update(frameTime);
			_engine.Render();



			//shaderDefault->Bind();
			////transform = glm::rotate(transform, 0.02f, rotation);
			//shaderDefault->SetUniformMat4("u_transform", transform);
			//shaderDefault->SetUniformMat4("u_projection", projection);
			//shaderDefault->SetUniformMat4("u_view", camera.GetViewMatrix());
			//for (auto m : *model)
			//{
			//	m.BindBuffer();
			//	glDrawElements(GL_TRIANGLES, (int)m.Indices.size(), GL_UNSIGNED_INT, nullptr);
			//	m.UnbindBuffer();
			//}
			//shaderDefault->Unbind();



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
