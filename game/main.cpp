#include "game.h"

using namespace Game;

Bitmap bm_test(L"../test.bmp");
Sprite sp_test(50, 0, 50, 100, 0, &bm_test);
PureColor pc_bg(RGB(200, 255, 255));
Sprite sp_bg(0, 0, 384, 288, 100, &pc_bg);
Scene sc_test = { &sp_test, &sp_bg };

LRESULT paint(EventHandler *self, DrawingContext dc) {
	sc_test.paintOn(dc.hdc);
	return 0;
}

LRESULT click(EventHandler *self, HWND hWnd, WPARAM wParam, LPARAM lParam) {
	sc_test.removeSprite(&sp_test);
	self->markDirty();
	return 0;
}

int APIENTRY wWinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine, int nCmdShow
) {
	initGame(hInstance);
	event_handler.addHandler(WM_PAINT, &paint);
	event_handler.addHandler(WM_LBUTTONDOWN, &click);
	sc_test.sortSprites();
	return runGame();
}