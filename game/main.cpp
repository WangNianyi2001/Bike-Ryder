#include "game.h"

using namespace Game;

Bitmap bm_test(L"../test.bmp");
Sprite sp_test(50, 0, 50, 100, 0, &bm_test);
PureColor pc_bg(RGB(200, 255, 255));
Sprite sp_bg(0, 0, 384, 288, 100, &pc_bg);
Scene sc_test;

void scTestInit() {
	sc_test.addSprite(&sp_test);
	sc_test.addSprite(&sp_bg);
	sc_test.sortSprites();
}

LRESULT paint(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	sc_test.paintOn(hdc);
	EndPaint(hWnd, &ps);
	return 0;
}

LRESULT click(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	// sc_test.removeSprite(&sp_test);
	sp_test.visible = false;
	InvalidateRect(hWnd, NULL, true);
	return 0;
}

int APIENTRY wWinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine, int nCmdShow
) {
	initGame(hInstance);
	// Initialization for scene test
	sc_test.init = &scTestInit;
	sc_test.addHandler(WM_PAINT, &paint);
	sc_test.addHandler(WM_LBUTTONDOWN, &click);
	addScene("test", &sc_test);
	// Load the scene
	loadScene("test");
	return runGame();
}