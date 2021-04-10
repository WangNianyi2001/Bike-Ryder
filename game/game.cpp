#include "game.h"

namespace Game {
	LRESULT CALLBACK eventProcessor(HWND hWnd, UINT type, WPARAM wParam, LPARAM lParam) {
		if(type == WM_DESTROY)
			PostQuitMessage(0);
		return DefWindowProc(hWnd, type, wParam, lParam);
	}

	void initGame(HINSTANCE hInstance) {
		window = new Window(hInstance, Window::InitArg{
			.title = L"Crazy Bicycle",
			.width = vwidth * scale,
			.height = vheight * scale,
			.event_processor = eventProcessor,
		});
		SetTimer(window->window, 0, 1000 / fps, NULL);
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