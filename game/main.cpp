#include "game.h"

using namespace SimpleWin32;

Window *window;
EventHandler event_handler;
PureColor vscreen(RGB(255, 255, 255), { vwidth, vheight });

int y = 10;
PureColor red(RGB(255, 0, 0), { 8, 8 });
Text text("something", red);

LRESULT paint(EventHandler *self, DrawingContext dc) {
	text.paintOn(vscreen.hdc, { 10, y });
	vscreen.paintOn(
		dc.hdc,
		{ 0, 0 },
		{ vwidth * pixel_scale, vheight * pixel_scale },
		SRCCOPY
	);
	return 0;
}

LRESULT timer(EventHandler *self, HWND hWnd, WPARAM wParam, LPARAM lParam) {
	++y;
	self->markDirty();
	return 0;
}

LRESULT CALLBACK eventProcessor(HWND hWnd, UINT type, WPARAM wParam, LPARAM lParam) {
	return event_handler(hWnd, type, wParam, lParam);
}

int APIENTRY wWinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine, int nCmdShow
) {
	srand((unsigned)GetTickCount64());

	window = new Window(hInstance, Window::InitArg{
		.title = L"�ų� Bike Ryder",
		.width = vwidth * pixel_scale,
		.height = vheight * pixel_scale,
		.style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
		.event_processor = eventProcessor,
	});
	SetTimer(window->window, 0, 1000 / fps, NULL);
	event_handler.setMedian(WM_PAINT, EventHandler::defaultPaintMedian);
	event_handler.addHandler(WM_PAINT, paint);
	event_handler.addHandler(WM_TIMER, timer);
	event_handler.addHandler(WM_DESTROY, EventHandler::defaultDestroyHandler);

	return window->run();
}