#include "game.h"

using namespace Game;

int APIENTRY wWinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine, int nCmdShow
) {
	initGame(hInstance);
	return runGame();
}