#include "scene.h"

Game::Scene level;
void levelInit();

using namespace Game;

Bitmap bm_test(L"../test.bmp");
Sprite sp_test(50, 0, 50, 100, 0, &bm_test);
PureColor pc_bg(RGB(200, 255, 255));
Sprite sp_bg(0, 0, 384, 288, 100, &pc_bg);

void enter() {
	level.addSprite(&sp_test);
	level.addSprite(&sp_bg);
	level.sortSprites();
}

LRESULT paint(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	level.paintOn(hdc);
	EndPaint(hWnd, &ps);
	return 0;
}

LRESULT click(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	// sc_test.removeSprite(&sp_test);
	sp_test.visible = false;
	InvalidateRect(hWnd, NULL, true);
	return 0;
}

void levelInit() {
	level.onEnter = &enter;
	level.addHandler(WM_PAINT, &paint);
	level.addHandler(WM_LBUTTONDOWN, &click);
}