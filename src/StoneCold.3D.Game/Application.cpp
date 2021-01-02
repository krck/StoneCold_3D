
#include "GameCore.hpp"

using namespace StoneCold::Game;

//
// Referenced Projects
// - StoneCold.Engine
// - StoneCold.Resources
// 
// Using Static Libraries (.libs) instead of Dynamic Libraries (.dlls)
// to ensure all StoneCold source code will be put into the .exe file
// and only external code (SDL2, ...) needs to be added dynamically
//

int main(int argc, const char* argv[]) {
	// Setup the game and start the main loop
	GameCore game;
	return (game.Initialize("StoneCold") ? game.Run() : -1);
}
