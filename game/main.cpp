#include "SimpleWin32Lib.h"
#include "game.h"

using namespace SimpleWin32;

Window *window;
EventHandler event_handler;
PureColor vscreen(RGB(255, 255, 255), { vwidth, vheight });

void projectOnto(HDC hdc) {
	vscreen.paintOn(
		hdc,
		{ 0, 0 },
		{ vwidth * pixel_scale, vheight * pixel_scale },
		SRCCOPY
	);
}

LRESULT paint(EventHandler *self, DrawingContext dc) {
	background.paintOn(vscreen.hdc, { 0, 0 });
	for(auto p : Physics::all)
		p->render(vscreen.hdc);
	projectOnto(dc.hdc);
	return 0;
}

LRESULT timer(EventHandler *self, HWND hWnd, WPARAM wParam, LPARAM lParam) {
	update();
	if(rand() % 100 <= 10 * player.vz)
		generateNPC();
	self->markDirty();
	return 0;
}
LRESULT keyup(EventHandler *self, HWND hWnd, WPARAM wParam, LPARAM lParam) {
	updateMoveState(wParam, false);
	return 0;
}

LRESULT keydown(EventHandler *self, HWND hWnd, WPARAM wParam, LPARAM lParam) {
	updateMoveState(wParam, true);
	kick(wParam);
	if(wParam == VK_SPACE)
		player.ride();
	return 0;
}

LRESULT CALLBACK eventProcessor(HWND hWnd, UINT type, WPARAM wParam, LPARAM lParam) {
	return event_handler(hWnd, type, wParam, lParam);
}

int APIENTRY wWinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine, int nCmdShow
) {
	// Initializing environment
	srand((unsigned)GetTickCount64());

	// Setting up the window
	window = new Window(hInstance, Window::InitArg{
		.title = L"µÅ³µ Bike Ryder",
		.width = vwidth * pixel_scale,
		.height = vheight * pixel_scale,
		.style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
		.event_processor = eventProcessor,
	});
	SetTimer(window->window, 0, 1000 / fps, NULL);
	event_handler.setMedian(WM_PAINT, EventHandler::defaultPaintMedian);
	event_handler.addHandler(WM_PAINT, paint);
	event_handler.addHandler(WM_TIMER, timer);
	event_handler.addHandler(WM_KEYDOWN, keydown);
	event_handler.addHandler(WM_KEYUP, keyup);
	event_handler.addHandler(WM_DESTROY, EventHandler::defaultDestroyHandler);

	// Initializing the game
	initGame();

	// Putting the window to run
	int result = window->run();

	// Cleaning
	unloadImages();
	return result;
}