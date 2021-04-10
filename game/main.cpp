#include "game.h"
#include "resources.h"

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

Character player(self_fall, self_kick, self_fall);

PureColor background(RGB(255, 255, 255));

LRESULT paint(EventHandler *self, DrawingContext dc) {
	background.paintOn(vscreen.hdc, { 0, 0 });
	player.render(vscreen.hdc);
	projectOnto(dc.hdc);
	return 0;
}

LRESULT timer(EventHandler *self, HWND hWnd, WPARAM wParam, LPARAM lParam) {
	((Animation *)player.current->second)->update();
	self->markDirty();
	return 0;
}

LRESULT keydown(EventHandler *self, HWND hWnd, WPARAM wParam, LPARAM lParam) {
	switch(wParam) {
	case 'A':
		player.kick(1);
		break;
	case 'D':
		player.kick(-1);
	}
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
		.style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
		.event_processor = eventProcessor,
	});
	SetTimer(window->window, 0, 1000 / fps, NULL);
	event_handler.setMedian(WM_PAINT, EventHandler::defaultPaintMedian);
	event_handler.addHandler(WM_PAINT, paint);
	event_handler.addHandler(WM_TIMER, timer);
	event_handler.addHandler(WM_KEYDOWN, keydown);
	event_handler.addHandler(WM_DESTROY, EventHandler::defaultDestroyHandler);

	// Animations
	player.z = 1.0f;

	// Set the window to run
	int result = window->run();

	// Cleaning
	unloadImages();
	return result;
}