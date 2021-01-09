
#include "SimulationManager.hpp"

using namespace StoneCold;
using namespace StoneCold::Engine;
using namespace StoneCold::Resources;
using namespace StoneCold::Game;


bool StoneCold::Game::SimulationManager::Initialize(EngineCore* engine, WindowManager* window, ResourceManager* resourceManager, MapManager* mapManager) {
	if (engine != nullptr && window != nullptr && resourceManager != nullptr && mapManager != nullptr) {
		_engine = engine;
		_window = window;
		_resourceManager = resourceManager;
		_mapManager = mapManager;
		return true;
	}
	else {
		return false;
	}
}


void SimulationManager::CreateIntroState() {
	try {
		// First clear the Intro Resources
		_resourceManager->UnloadResources(ResourceLifeTime::Intro);
		_engine->RemoveState<IntroState>();

		// Create a new IntroState
		auto intro = std::make_shared<IntroState>(5, _engine);
		auto introECS = intro->GetECS();
		intro->Initialize();

		//// Get all external Resources needed by the IntroState (Background image, Font, ...)
		//auto backgroundTexture = _resourceManager->LoadExternalResource<TextureResource>(ResourceLifeTime::Intro, Raw::BACKGROUND_IMAGE);
		//auto fontTTF = _resourceManager->LoadExternalResource<FontResource>(ResourceLifeTime::Intro, Raw::FONT_CROM);
		//// Get all internal/object Data needed by the IntroState
		//// auto backgroundData = StoneCold::Resources::Raw::FullscreenBackground;

		//// -----------------------------------------------------
		//// ------- INTRO - GAME-OBJECT - Background Image ------
		//// -----------------------------------------------------
		//auto background = introECS->CreateEntity();
		//SDL_Rect backgroundSrc = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
		//SDL_FRect backgroundDest = { 0.f, 0.f, FWINDOW_WIDTH, FWINDOW_HEIGHT };
		//introECS->AddAdditionalSystemMask(background, RENDER_STATIC);
		//introECS->AddComponent<ScreenPositionComponent>(background, { backgroundSrc, backgroundDest });
		//introECS->AddComponent<SpriteComponent>(background, { backgroundTexture->GetTextureSDL(), SDL_RendererFlip::SDL_FLIP_NONE });

		//// -----------------------------------------------------
		//// --- INTRO - GUI-OBJECT - Label "Press any Button" ---
		//// -----------------------------------------------------
		//auto label = introECS->CreateEntity();
		//const std::string labelText = "Press any Button to start ...";
		//const std::string labelName = "Label_Intro_Press_Any_Button";
		//auto lbTex = _resourceManager->CreateFontTexture(ResourceLifeTime::Intro, labelName, fontTTF->GetFontBig(), labelText, CL_BLACK);
		//auto surf = lbTex->SurfaceSize;
		//SDL_Rect srcRect = { 0, 0, surf.X, surf.Y };
		//SDL_FRect destRect = { (WINDOW_WIDTH / 2.f) - (surf.X / 2.f), 500.f, static_cast<float>(surf.X), static_cast<float>(surf.Y) };
		//introECS->AddAdditionalSystemMask(label, RENDER_STATIC);
		//introECS->AddComponent<ScreenPositionComponent>(label, { srcRect, destRect });
		//introECS->AddComponent<SpriteComponent>(label, { lbTex->GetTextureSDL(), SDL_RendererFlip::SDL_FLIP_NONE });

		// Finally add the new IntroState to the Engines States
		_engine->AddState<IntroState>(intro);
	}
	catch (const std::exception & ex) {
		std::cout << "Loading the Intro state failed:\n" << ex.what() << std::endl;
	}
}


void SimulationManager::CreateGameState() {
	try {
		// First clear the Game Resources
		_resourceManager->UnloadResources(ResourceLifeTime::Game);
		_engine->RemoveState<GameState>();

		// Get the default Projection Matrix to render the Game based on Window aspect, FoV and "near/far" values
		glm::mat4 projection = glm::perspective(45.0f, (_window->GetWidth() / (float)_window->GetHeight()), 0.1f, 1000.0f);

		// Create a new GameState
		auto game = std::make_shared<GameState>(20000, _engine, projection);
		auto gameECS = game->GetECS();
		game->Initialize();

		// Get all external Resources needed by the GameState (Player Character, Player GUI, etc.)
		auto coordsModel = _resourceManager->LoadResource<MeshResource>(ResourceLifeTime::Game, "models\\coordinates\\coordinates.obj");
		auto cubeModel = _resourceManager->LoadResource<ModelResource>(ResourceLifeTime::Game, "models\\map_cube\\map_cube.obj");

		// -----------------------------------------------------
		// ----------- GAME - GAME-OBJECT - Map Tiles ----------
		// -----------------------------------------------------
		auto map = std::move(_mapManager->GenerateMap(LevelType::Grassland, glm::ivec2(100, 100)));
		for (const auto& m : map) {
			// Cube Model data and ECS Components
			auto cube = gameECS->CreateEntity();
			auto cubeMesh = cubeModel->Model[0];
			auto cubeTransformation = glm::mat4(1.0f);
			cubeTransformation = glm::scale(cubeTransformation, glm::vec3(1.0f));
			cubeTransformation = glm::translate(cubeTransformation, glm::vec3(m.x, m.z, m.y));
			//transformation = glm::rotate(transformation, t.Angle, t.Rotation);s
			gameECS->AddAdditionalSystemMask(cube, MASK_SHADER_DEFAULT);
			gameECS->AddComponent<MeshComponent>(cube, { cubeMesh.GetVAO(), cubeMesh.GetEBO(), cubeMesh.GetSize() });
			gameECS->AddComponent<TextureComponent>(cube, { cubeMesh.Texture->GetTextureId(), cubeMesh.Texture->GetNormalMapId(), cubeMesh.Texture->GetSpecularMapId() });
			gameECS->AddComponent<TransformationComponent>(cube, { cubeTransformation });
		}

		// -----------------------------------------------------
		// ------- GAME - GAME-OBJECT - Player Character -------
		// -----------------------------------------------------
		// Coordinates Model data and ECS Components
		auto coordinates = gameECS->CreateEntity();
		auto coordTransformation = glm::mat4(1.0f);
		coordTransformation = glm::scale(coordTransformation, glm::vec3(1.0f));
		coordTransformation = glm::translate(coordTransformation, glm::vec3(0.0f, 0.0f, 0.0f));
		//transformation = glm::rotate(transformation, t.Angle, t.Rotation);s
		gameECS->AddAdditionalSystemMask(coordinates, MASK_SHADER_DEFAULTNOTEX);
		gameECS->AddComponent<MeshComponent>(coordinates, { coordsModel->GetVAO(), coordsModel->GetEBO(), coordsModel->GetSize() });
		gameECS->AddComponent<TransformationComponent>(coordinates, { coordTransformation });

		//game->SetEntities(player, mapTiles);

		// Finally add the new GameState to the Engines States
		_engine->AddState<GameState>(game);
	}
	catch (const std::exception & ex) {
		std::cout << "Loading the Game state failed:\n" << ex.what() << std::endl;
	}
}


void SimulationManager::CreateMenuState() {
	try {
		// First clear the Menu Resources
		_resourceManager->UnloadResources(ResourceLifeTime::Menu);
		_engine->RemoveState<MenuState>();

		// Create a new MenuState
		auto menu = std::make_shared<MenuState>(10, _engine);
		auto menuECS = menu->GetECS();
		menu->Initialize();

		//// Get all external Resources needed by the MenuState (Background image, Font, ...)
		//auto backgroundTexture = _resourceManager->LoadExternalResource<TextureResource>(ResourceLifeTime::Menu, Raw::BACKGROUND_IMAGE);
		//auto stonecoldTexture = _resourceManager->LoadExternalResource<TextureResource>(ResourceLifeTime::Menu, Raw::STONECOLD_LOGO);
		//auto guiTexture = _resourceManager->LoadExternalResource<TextureResource>(ResourceLifeTime::Menu, Raw::GUI_TEXTURE);
		//auto fontTTF = _resourceManager->LoadExternalResource<FontResource>(ResourceLifeTime::Menu, Raw::FONT_CROM);

		//// -----------------------------------------------------
		//// ------- MENU - GAME-OBJECT - Background Image -------
		//// -----------------------------------------------------
		//auto background = menuECS->CreateEntity();
		//SDL_Rect backgroundSrc = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
		//SDL_FRect backgroundDest = { 0.f, 0.f, FWINDOW_WIDTH, FWINDOW_HEIGHT };
		//menuECS->AddAdditionalSystemMask(background, RENDER_STATIC);
		//menuECS->AddComponent<ScreenPositionComponent>(background, { backgroundSrc, backgroundDest });
		//menuECS->AddComponent<SpriteComponent>(background, { backgroundTexture->GetTextureSDL(), SDL_RendererFlip::SDL_FLIP_NONE });

		//// -----------------------------------------------------
		//// -------- MENU - GAME-OBJECT - StoneCold Logo --------
		//// -----------------------------------------------------
		//auto logo = menuECS->CreateEntity();
		//SDL_Rect logoSrc = { 0, 0, stonecoldTexture->SurfaceSize.X, stonecoldTexture->SurfaceSize.Y };
		//SDL_FRect logoDest = { (WINDOW_WIDTH / 2.f) - 250.f, 100.f, 500.f, 100.f };
		//menuECS->AddAdditionalSystemMask(logo, RENDER_STATIC);
		//menuECS->AddComponent<ScreenPositionComponent>(logo, { logoSrc, logoDest });
		//menuECS->AddComponent<SpriteComponent>(logo, { stonecoldTexture->GetTextureSDL(), SDL_RendererFlip::SDL_FLIP_NONE });

		//// -----------------------------------------------------
		//// --------- MENU - GUI-OBJECT - Button "Play" ---------
		//// -----------------------------------------------------
		//auto btnPlay = menuECS->CreateEntity();
		//auto btnFlip = SDL_RendererFlip::SDL_FLIP_NONE;
		//auto btnDefaultAnim = &Raw::BUTTON_ANIMATION_FRAMES.find("idle")->second;
		//auto btnDefaultSrc = btnDefaultAnim->FramePositions.front();
		//auto btnContentPlay = _resourceManager->CreateFontTexture(ResourceLifeTime::Menu, "Button_Menu_Start", fontTTF->GetFontBig(), "Play", CL_BLACK);
		//SDL_FRect destPlay = { (WINDOW_WIDTH / 2.f) - 100.f, 300.f, 200.f, 50.f };
		//// Set the Source and Destination of the Content. Dest: Content is always centered within the Button
		//auto centerPlayBtn = Vec2(destPlay.x + (destPlay.w / 2.f), destPlay.y + (destPlay.h / 2.f) - 4.f);
		//SDL_Rect playContentSrc = { 0, 0, static_cast<int>(btnContentPlay->SurfaceSize.X), static_cast<int>(btnContentPlay->SurfaceSize.Y) };
		//SDL_FRect playContentDest = { centerPlayBtn.X - (btnContentPlay->SurfaceSize.X / 2.f), centerPlayBtn.Y - (btnContentPlay->SurfaceSize.Y / 2.f) + 1,
		//							  static_cast<float>(btnContentPlay->SurfaceSize.X), static_cast<float>(btnContentPlay->SurfaceSize.Y) };
		//// Add the Button Components (Trans, Moving and Animation are the construct for switching Hover/Non-Hover
		//// The SpriteComponentFixed is used to display the content (Text, Image, ...) within the Button 
		//menuECS->AddComponent<AttributeComponentUI>(btnPlay, { UiElementAttribute::UIE_Idle });
		//menuECS->AddComponent<AnimationComponent>(btnPlay, { &Raw::BUTTON_ANIMATION_FRAMES, btnDefaultAnim, 0, 0 });
		//menuECS->AddComponent<ScreenPositionLayeredComponent>(btnPlay, { btnDefaultSrc, destPlay, playContentSrc, playContentDest });
		//menuECS->AddComponent<SpriteLayeredComponent>(btnPlay, { guiTexture->GetTextureSDL(), btnFlip, btnContentPlay->GetTextureSDL(), btnFlip });

		//// -----------------------------------------------------
		//// ------- MENU - GUI-OBJECT - Button "Options" --------
		//// -----------------------------------------------------
		//auto btnOptions = menuECS->CreateEntity();
		//auto btnContentOptions = _resourceManager->CreateFontTexture(ResourceLifeTime::Menu, "Button_Menu_Options", fontTTF->GetFontBig(), "Options", CL_BLACK);
		//SDL_FRect destOptions = { (WINDOW_WIDTH / 2.f) - 100.f, 370.f, 200.f, 50.f };
		//// Set the Source and Destination of the Content. Dest: Content is always centered within the Button
		//auto centerOptionsBtn = Vec2(destOptions.x + (destOptions.w / 2.f), destOptions.y + (destOptions.h / 2.f) - 4.f);
		//SDL_Rect optContentSrc = { 0, 0, static_cast<int>(btnContentOptions->SurfaceSize.X), static_cast<int>(btnContentOptions->SurfaceSize.Y) };
		//SDL_FRect optContentDest = { centerOptionsBtn.X - (btnContentOptions->SurfaceSize.X / 2.f), centerOptionsBtn.Y - (btnContentOptions->SurfaceSize.Y / 2.f) + 1,
		//							 static_cast<float>(btnContentOptions->SurfaceSize.X), static_cast<float>(btnContentOptions->SurfaceSize.Y) };
		//menuECS->AddComponent<AttributeComponentUI>(btnOptions, { UiElementAttribute::UIE_Idle });
		//menuECS->AddComponent<AnimationComponent>(btnOptions, { &Raw::BUTTON_ANIMATION_FRAMES, btnDefaultAnim, 0, 0 });
		//menuECS->AddComponent<ScreenPositionLayeredComponent>(btnOptions, { btnDefaultSrc, destOptions, optContentSrc, optContentDest });
		//menuECS->AddComponent<SpriteLayeredComponent>(btnOptions, { guiTexture->GetTextureSDL(), btnFlip, btnContentOptions->GetTextureSDL(), btnFlip });

		//// -----------------------------------------------------
		//// ------- MENU - GUI-OBJECT - Button "Credits" --------
		//// -----------------------------------------------------
		//auto btnCredits = menuECS->CreateEntity();
		//auto btnContentCredits = _resourceManager->CreateFontTexture(ResourceLifeTime::Menu, "Button_Menu_Credits", fontTTF->GetFontBig(), "Credits", CL_BLACK);
		//SDL_FRect destCredits = { (WINDOW_WIDTH / 2.f) - 100.f, 440.f, 200.f, 50.f };
		//// Set the Source and Destination of the Content. Dest: Content is always centered within the Button
		//auto centerCreditsBtn = Vec2(destCredits.x + (destCredits.w / 2.f), destCredits.y + (destCredits.h / 2.f) - 4.f);
		//SDL_Rect creditContentSrc = { 0, 0, static_cast<int>(btnContentCredits->SurfaceSize.X), static_cast<int>(btnContentCredits->SurfaceSize.Y) };
		//SDL_FRect creditContentDest = { centerCreditsBtn.X - (btnContentCredits->SurfaceSize.X / 2.f), centerCreditsBtn.Y - (btnContentCredits->SurfaceSize.Y / 2.f) + 1,
		//								static_cast<float>(btnContentCredits->SurfaceSize.X), static_cast<float>(btnContentCredits->SurfaceSize.Y) };
		//menuECS->AddComponent<AttributeComponentUI>(btnCredits, { UiElementAttribute::UIE_Idle });
		//menuECS->AddComponent<AnimationComponent>(btnCredits, { &Raw::BUTTON_ANIMATION_FRAMES, btnDefaultAnim, 0, 0 });
		//menuECS->AddComponent<ScreenPositionLayeredComponent>(btnCredits, { btnDefaultSrc, destCredits, creditContentSrc, creditContentDest });
		//menuECS->AddComponent<SpriteLayeredComponent>(btnCredits, { guiTexture->GetTextureSDL(), btnFlip, btnContentCredits->GetTextureSDL(), btnFlip });

		//// -----------------------------------------------------
		//// --------- MENU - GUI-OBJECT - Button "Quit" ---------
		//// -----------------------------------------------------
		//auto btnQuit = menuECS->CreateEntity();
		//auto btnContentQuit = _resourceManager->CreateFontTexture(ResourceLifeTime::Menu, "Button_Menu_Quit", fontTTF->GetFontBig(), "Quit", CL_BLACK);
		//SDL_FRect destQuit = { (WINDOW_WIDTH / 2.f) - 100.f, 510.f, 200.f, 50.f };
		//// Set the Source and Destination of the Content. Dest: Content is always centered within the Button
		//auto centerQuitBtn = Vec2(destQuit.x + (destQuit.w / 2.f), destQuit.y + (destQuit.h / 2.f) - 4.f);
		//SDL_Rect quitContentSrc = { 0, 0, static_cast<int>(btnContentQuit->SurfaceSize.X), static_cast<int>(btnContentQuit->SurfaceSize.Y) };
		//SDL_FRect quitontentDest = { centerQuitBtn.X - (btnContentQuit->SurfaceSize.X / 2.f), centerQuitBtn.Y - (btnContentQuit->SurfaceSize.Y / 2.f) + 1,
		//							 static_cast<float>(btnContentQuit->SurfaceSize.X), static_cast<float>(btnContentQuit->SurfaceSize.Y) };
		//menuECS->AddComponent<AttributeComponentUI>(btnQuit, { UiElementAttribute::UIE_Idle });
		//menuECS->AddComponent<AnimationComponent>(btnQuit, { &Raw::BUTTON_ANIMATION_FRAMES, btnDefaultAnim, 0, 0 });
		//menuECS->AddComponent<ScreenPositionLayeredComponent>(btnQuit, { btnDefaultSrc, destQuit, quitContentSrc, quitontentDest });
		//menuECS->AddComponent<SpriteLayeredComponent>(btnQuit, { guiTexture->GetTextureSDL(), btnFlip, btnContentQuit->GetTextureSDL(), btnFlip });

		//menu->SetButtons({ btnPlay, btnOptions, btnCredits, btnQuit });

		// Finally add the new MenuState to the Engines States
		_engine->AddState<MenuState>(menu);
	}
	catch (const std::exception & ex) {
		std::cout << "Loading the Menu state failed:\n" << ex.what() << std::endl;
	}
}


void SimulationManager::LoadLevel() {
	try {
		// First clear the Level Resources and check, if the Engine has a GameState that runs the Level
		_resourceManager->UnloadResources(ResourceLifeTime::Level);
		if (_engine->HasState<GameState>()) {
			auto gameState = _engine->GetState<GameState>();
			auto gameECS = gameState->GetECS();
			auto mapTiles = gameState->GetMapTiles();

			//// -----------------------------------------------------
			//// ---------- PROCEDURALLY GENERATE A NEW MAP ----------
			//// -----------------------------------------------------
			//// Get a new, random Map Texture
			//const auto levelType = (LevelType)(rand() % 5 + 0);
			//std::string texturePath = Raw::MAP_TEXTURES.find(levelType)->second;
			//auto mapTexture = _resourceManager->LoadExternalResource<TextureResource>(ResourceLifeTime::Level, texturePath);
			//auto skeletonTexture = _resourceManager->LoadExternalResource<TextureResource>(ResourceLifeTime::Level, Raw::SKELETON_TEXTURE);
			//// Pre-defined map patterns
			//std::pair<float, float> pattern = { 0.f, 0.f };
			//const std::vector<std::pair<float, float>> mapPatterns = {
			//	{ 0.6f, 0.01f }, // 1) ROCKY: Map with few open spaces, very jagged edges and lots of corridors and obstacles
			//	{ 0.75f, 0.1f }, // 2) MIXED: Map with bigger open spaces, some jagged edges and few obstacles
			//	{ 0.95f, 0.2f }	 // 3) FLAT: Map with one massive, open space, smoothe-ish edges and no corridors/obstacles
			//};
			//// Grassland and Arctic can be FLAT or MIXED, Highlands or Desert can be ROCKY or MIXED, Castle can only be ROCKY
			//if (levelType == LevelType::Grassland || levelType == LevelType::Arctic) { pattern = mapPatterns[(rand() & 1) + 1]; }
			//else if (levelType == LevelType::Highlands || levelType == LevelType::Desert) { pattern = mapPatterns[(rand() & 1)]; }
			//else { pattern = mapPatterns[0]; }
			//// Get a new, procedurally generated Map Layout
			//auto mapLayout = _mapManager->GenerateMap(Vec2i(MAP_SIZE, MAP_SIZE), pattern.first, pattern.second);
			//auto spawnPos = _mapManager->GetStartEndPositions().first;

			//// -----------------------------------------------------
			//// --------------- CREATE THE MAP TILES ----------------
			//// -----------------------------------------------------
			//auto& _positionComponents = *gameECS->GetComponentArray<ScreenPositionComponent>();
			//auto& _spriteComponents = *gameECS->GetComponentArray<SpriteComponent>();
			//for (uint64 row = 0; row < mapLayout.size(); row++) {
			//	for (uint64 column = 0; column < mapLayout[row].size(); column++) {
			//		// Map tile position based on row/column within the mapLayout
			//		const auto type = Raw::MAP_TILE_FRAMES.find(mapLayout[row][column]);
			//		const auto frame = type->second;
			//		// Get the Entity and its Components
			//		const auto entityId = mapTiles[row * MAP_SIZE + column];
			//		auto& pos = _positionComponents[entityId];
			//		auto& spr = _spriteComponents[entityId];

			//		// Update the Source rectange frame inside the texture (Pixels to take, from the .png)
			//		// Update the Destination rectangle with the actual position on screen with scaling (Where to put the Pixels)
			//		pos.SrcRect = frame.first;
			//		pos.DestRect = { column * 96.f, row * 96.f, static_cast<float>(pos.SrcRect.w * 3), static_cast<float>(pos.SrcRect.h * 3) };
			//		// Update the MapTile Texture (and texture flip)
			//		spr.Texture = mapTexture->GetTextureSDL();
			//		spr.Flip = frame.second;

			//		// Add the CollisionComponent in case its a Wall-Tile
			//		if (static_cast<int>(type->first) > static_cast<int>(MapTileTypes::Floor_Shadow))
			//			gameECS->AddComponent<CollisionComponent>(entityId, { 2, Vec2(pos.DestRect.w, pos.DestRect.h), pos.DestRect, nullptr });
			//		else
			//			gameECS->RemoveComponent<CollisionComponent>(entityId);
			//	}
			//}

			//// -----------------------------------------------------
			//// ---------------- SPAWN THE MONSTERS -----------------
			//// -----------------------------------------------------
			//// ...
			//// Skeleton dimensions
			//uint32 scale = 3;
			//auto position = Vec2(spawnPos.X * 96.f, spawnPos.Y * 96.f);
			//auto dimension = Vec2(32, 32);
			//auto skeletonDefaultAnim = &Raw::SKELETON_ANIMATION_FRAMES.find("idle")->second;
			//// Set the Source rectange frame inside the texture (Pixels to take, from the .png)
			//// Set the Destination rectangle with the actual position on screen with scaling (Where to put the Pixels)
			//SDL_Rect defaultSrcRect = { 0, 0, static_cast<int>(dimension.X), static_cast<int>(dimension.Y) };
			//SDL_FRect defaultDestRect = { position.X, position.Y, dimension.X * scale, dimension.Y * scale };
			//auto skeleton = gameECS->CreateEntity();
			//gameECS->AddAdditionalSystemMask(skeleton, RENDER_MOTION);
			//gameECS->AddComponent<VelocityComponent>(skeleton, { Vec2() });
			//gameECS->AddComponent<TransformationComponent>(skeleton, { position, Vec2(), dimension, 250, 250, scale });
			//gameECS->AddComponent<CollisionComponent>(skeleton, { 1, Vec2(14.f, 18.f), defaultDestRect, nullptr });
			//gameECS->AddComponent<AnimationComponent>(skeleton, { &Raw::SKELETON_ANIMATION_FRAMES, skeletonDefaultAnim, 0, 0 });
			//gameECS->AddComponent<ScreenPositionComponent>(skeleton, { defaultSrcRect, defaultDestRect });
			//gameECS->AddComponent<SpriteComponent>(skeleton, { skeletonTexture->GetTextureSDL(), SDL_RendererFlip::SDL_FLIP_NONE });


			// Set Players new spawn position within the Level
			//gameState->SetSpawnPosition({ spawnPos.X * 96, spawnPos.Y * 96 });
		}
	}
	catch (const std::exception & ex) {
		std::cout << "Loading a new Level failed:\n" << ex.what() << std::endl;
	}
}
