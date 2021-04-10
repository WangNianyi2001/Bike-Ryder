#include "SimpleWin32Lib.h"
#include "config.h"

using namespace SimpleWin32;

LRESULT CALLBACK eventProcessor(HWND hWnd, UINT type, WPARAM wParam, LPARAM lParam) {
	if(type == WM_DESTROY)
		PostQuitMessage(0);
	return DefWindowProc(hWnd, type, wParam, lParam);
}

static Window *window;

int APIENTRY wWinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine, int nCmdShow
) {
	srand((unsigned)GetTickCount64());

	window = new Window(hInstance, Window::InitArg{
		.title = L"我是安踏王",
		.width = vwidth * pixel_scale,
		.height = vheight * pixel_scale,
		.event_processor = eventProcessor,
	});
	SetTimer(window->window, 0, 1000 / fps, NULL);

	return window->run();
}