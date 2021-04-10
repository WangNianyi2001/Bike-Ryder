#include "game.h"

namespace Game {
	LRESULT CALLBACK eventProcessor(HWND hWnd, UINT type, WPARAM wParam, LPARAM lParam) {
		if(type == WM_DESTROY)
			PostQuitMessage(0);
		if(!active_scene)
			return DefWindowProc(hWnd, type, wParam, lParam);
		if(!active_scene->handlers.count(type))
			return DefWindowProc(hWnd, type, wParam, lParam);
		return active_scene->handlers[type](hWnd, wParam, lParam);
	}

	void addScene(string name, Scene *scene) {
		scenes.insert(pair(name, scene));
	}

	void loadScene(string name) {
		Scene *const scene = scenes[name];
		if(scene->onEnter)
			scene->onEnter();
		active_scene = scene;
	}

	void initGame(HINSTANCE hInstance) {
		window = new Window(hInstance, Window::InitArg{
			.title = L"Crazy Bicycle",
			.width = vwidth * scale,
			.height = vheight * scale,
			.event_processor = eventProcessor,
			});
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