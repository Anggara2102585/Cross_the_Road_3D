#include "config.h"
#include "control/game_app.h"

int main() {

	int width = 1730;
	int height = 920;

	// initialize P-RNG
	srand(time(NULL));
	
	GameApp* app = new GameApp(width, height);
	
	gameState nextAction = gameState::CONTINUE;
	while (nextAction != gameState::QUIT) {
		nextAction = app->mainLoop();
	}

	delete app;

	return 0;
}