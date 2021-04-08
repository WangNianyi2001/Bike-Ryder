#include "SimpleWin32Lib.h"

using namespace SimpleWin32;

EventHandler event_handler;
LRESULT CALLBACK eventProcessor(HWND hWnd, UINT type, WPARAM wParam, LPARAM lParam) {
	return event_handler(hWnd, type, wParam, lParam);
}

int APIENTRY wWinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine, int nCmdShow
) {
	Window window(hInstance, Window::InitArg{
		.title = L"Calculator",
		.event_processor = &eventProcessor,
	});

	event_handler.addHandler(WM_DESTROY, &EventHandler::defaultDestroyHandler);

	return window.run();
}