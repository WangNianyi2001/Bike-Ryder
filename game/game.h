#pragma once

#include "SimpleWin32Lib.h"
#include "scene.h"

namespace Game {
	using namespace SimpleWin32;

	constexpr int vwidth = 384, vheight = 288;

	int scale = 2;

	EventHandler event_handler;
	LRESULT CALLBACK eventProcessor(HWND hWnd, UINT type, WPARAM wParam, LPARAM lParam) {
		return event_handler(hWnd, type, wParam, lParam);
	}

	Window *window;

	void initGame(HINSTANCE hInstance) {
		window = new Window(hInstance, Window::InitArg{
			.title = L"Crazy Bicycle",
			.width = vwidth * scale,
			.height = vheight * scale,
			.event_processor = eventProcessor,
		});
		event_handler.addHandler(WM_DESTROY, &EventHandler::defaultDestroyHandler);
		event_handler.setMedian(WM_PAINT, &EventHandler::defaultPaintMedian);
	}

	void rescale(int scale) {
		RECT rect;
		GetWindowRect(window->window, &rect);
		rect.right = rect.left + scale * vwidth;
		rect.bottom = rect.top + scale * vheight;
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
		Game::scale = scale;
	}

	int runGame() {
		return window->run();
	}
}