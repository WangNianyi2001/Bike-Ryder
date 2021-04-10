#include "game.h"
#include <sstream>

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

Animation fall({
	Frame{ 150, Texture(
		{ 35, 94 }, { 17, 94 },
		new Bitmap(L"../images/self/ride/0.bmp")
	) },
	Frame{ 75, Texture(
		{ 37, 95 }, { 18, 95 },
		new Bitmap(L"../images/self/ride/1.bmp")
	) },
	Frame{ 150, Texture(
		{ 35, 94 }, { 17, 94 },
		new Bitmap(L"../images/self/ride/2.bmp")
	) },
	Frame{ 75, Texture(
		{ 37, 95 }, { 18, 95 },
		new Bitmap(L"../images/self/ride/3.bmp")
	) },
}, true, true);

PureColor background(RGB(200, 255, 255), { 384, 288 });

LRESULT paint(EventHandler *self, DrawingContext dc) {
	background.paintOn(vscreen.hdc, { 0, 0 }, { 384, 288 }, SRCCOPY);
	fall.paintOn(vscreen.hdc, { 100, 100 });
	projectOnto(dc.hdc);
	return 0;
}

LRESULT timer(EventHandler *self, HWND hWnd, WPARAM wParam, LPARAM lParam) {
	fall.update();
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
	event_handler.addHandler(WM_DESTROY, EventHandler::defaultDestroyHandler);

	return window->run();
}