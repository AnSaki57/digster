#include "game.hpp"

int main () {
	game Game;
	Game.initialise();

	while (!WindowShouldClose()) {
		// Do frame-by-frame processing
		Game.frameProcess();

		// Event handling
		Game.handleEvents();
		
		// Drawing
		BeginDrawing();
		Game.draw();
		EndDrawing();
	}

	Game.close();
	CloseWindow();
	return 0;
}
