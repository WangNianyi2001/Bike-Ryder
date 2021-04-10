#include "game.h"

extern Game::Scene level;
extern void levelInit();

using namespace Game;

int APIENTRY wWinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine, int nCmdShow
) {
	initGame(hInstance);

	levelInit();
	addScene("level", &level);

	loadScene("level");

	return runGame();
}