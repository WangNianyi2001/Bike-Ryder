#pragma once

#include "SimpleWin32Lib.h"

namespace Game {
	using namespace SimpleWin32;

	constexpr int virtual_width = 384, virtual_height = 288;
	int scale = 2;

	EventHandler event_handler;
	LRESULT CALLBACK eventProcessor(HWND hWnd, UINT type, WPARAM wParam, LPARAM lParam) {
		return event_handler(hWnd, type, wParam, lParam);
	}

	Window *window;

	void initGame(HINSTANCE hInstance) {
		window = new Window(hInstance, Window::InitArg{
			.title = L"Crazy Bicycle",
			.width = virtual_width * scale,
			.height = virtual_height * scale,
			.event_processor = eventProcessor,
		});
		event_handler.addHandler(WM_DESTROY, EventHandler::defaultDestroyHandler);
	}

	int runGame() {
		return window->run();
	}
}