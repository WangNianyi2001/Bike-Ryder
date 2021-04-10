#pragma once

#include "SimpleWin32Lib.h"
#include "scene.h"
#include <map>
#include <string>

namespace Game {
	using namespace SimpleWin32;
	using namespace std;

	static constexpr int vwidth = 384, vheight = 288;
	static int scale = 2;
	static constexpr int fps = 15;

	static map<string, Scene *> scenes;
	static Scene *active_scene = nullptr;

	static Window *window;

	LRESULT CALLBACK eventProcessor(HWND hWnd, UINT type, WPARAM wParam, LPARAM lParam);

	void addScene(string name, Scene *scene);
	void loadScene(string name);
	void initGame(HINSTANCE hInstance);
	void rescale(int scale);
	int runGame();
}