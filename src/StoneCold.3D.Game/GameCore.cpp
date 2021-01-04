#include "GameCore.hpp"

#include "DefaultShaderNoTex.hpp"
#include "LoadManager.hpp"
#include <windows.h>
#include <Camera.hpp>

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
		const uint8 frameTimeSize = FRAME_SAMPLES;
		auto frameTimes = std::array<uint64, FRAME_SAMPLES>();
		uint64 frameCount = 0;
		float averageFPS = 0.f;






		// Get the current path to the running .exe
		// When NULL is passed to GetModuleHandle, the handle of the exe itself is returned
		char ownPth[MAX_PATH];
		HMODULE hModule = GetModuleHandle(NULL);
		if (hModule != NULL) {
			GetModuleFileName(hModule, ownPth, (sizeof(ownPth)));
		}
		// Create the relative path to the current assets folder
		auto basePath = std::string(ownPth);
		basePath = basePath.substr(0, basePath.find_last_of("\\") + 1);
		basePath += "res\\";

		StoneCold::Resources::LoadManager ml = StoneCold::Resources::LoadManager();
		auto model = ml.LoadSimpleModelFromFile(basePath + "models\\coordinates\\coordinates.obj");

		// Create a full Shader program with Vertex Shader and Fragment Shader
		auto shaderDefault = std::make_unique<StoneCold::Resources::DefaultShaderNoTex>();

		StoneCold::Engine::Camera camera = StoneCold::Engine::Camera(glm::vec3(0.0f, 10.0f, 10.f));
		glm::mat4 projection = glm::perspective(45.0f, (_windowManager.GetWidth() / (float)_windowManager.GetHeight()), 0.1f, 1000.0f);

		glm::mat4 transform = glm::mat4(1.0f);
		//transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -3.0f));
		transform = glm::scale(transform, glm::vec3(1.0f));

		glm::vec3 rotation = glm::vec3(0.0f, 1.0f, 0.0f);






		// Start the main loop
		while (!_windowManager.IsClosed())
		{
			timeStamp_new = GetTicks();
			frameTime = timeStamp_new - timeStamp_old;

			_windowManager.Clear();

			// Handle Input
			if (keyboard.IsKeyPressed(GLFW_KEY_A)) { camera.ChangePosition(StoneCold::Engine::MoveDirection::LEFT, 0.02f); }
			if (keyboard.IsKeyPressed(GLFW_KEY_D)) { camera.ChangePosition(StoneCold::Engine::MoveDirection::RIGHT, 0.02f); }
			if (keyboard.IsKeyPressed(GLFW_KEY_W)) { camera.ChangePosition(StoneCold::Engine::MoveDirection::FORWARD, 0.02f); }
			if (keyboard.IsKeyPressed(GLFW_KEY_S)) { camera.ChangePosition(StoneCold::Engine::MoveDirection::BACKWARD, 0.02f); }
			if (keyboard.IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) { camera.ChangePosition(StoneCold::Engine::MoveDirection::DOWN, 0.02f); }
			if (keyboard.IsKeyPressed(GLFW_KEY_SPACE)) { camera.ChangePosition(StoneCold::Engine::MoveDirection::UP, 0.02f); }
			//camera.ChangeLineOfSight((int)mouse.GetMouseX(), (int)mouse.GetMouseY(), true);






			shaderDefault->Bind();

			//transform = glm::rotate(transform, 0.02f, rotation);
			shaderDefault->SetUniformMat4("u_transform", transform);
			shaderDefault->SetUniformMat4("u_projection", projection);
			shaderDefault->SetUniformMat4("u_view", camera.GetViewMatrix());

			for (auto m : *model)
			{
				m.BindBuffer();
				glDrawElements(GL_TRIANGLES, (int)m.Indices.size(), GL_UNSIGNED_INT, nullptr);
				m.UnbindBuffer();
			}
			shaderDefault->Unbind();

			auto pos = camera.GetPosition();
			std::cout << "X: " << pos.x << "\tY: " << pos.y << "\tZ: " << pos.z << std::endl;
			






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
				//std::cout << "FPS: " << (uint16)averageFPS << "\n";
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
