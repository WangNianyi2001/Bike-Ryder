#pragma once

#include "SimpleWin32Lib.h"
#include <map>
#include <string>

namespace Game {
	using namespace SimpleWin32;
	using namespace std;

	static constexpr int vwidth = 384, vheight = 288;
	static int scale = 2;
	static constexpr int fps = 15;

	static Window *window;

	LRESULT CALLBACK eventProcessor(HWND hWnd, UINT type, WPARAM wParam, LPARAM lParam);

	void initGame(HINSTANCE hInstance);
	void rescale(int scale);
	int runGame();
}