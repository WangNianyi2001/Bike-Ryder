#pragma once

#include "texture.h"
#include <windows.h>
#include <vector>
#include <set>
#include <initializer_list>

using namespace std;

struct Frame {
	ULONGLONG interval;
	Texture texture;
};

struct Animation {
	ULONGLONG last_frame;
	vector<Frame> frames;
	vector<Frame>::iterator active;
	bool loop = false, stop = true;
	void (*onEnd)(Animation *);
	Animation(initializer_list<Frame> frames, bool loop = false, void (*onEnd)(Animation *) = nullptr);
	void begin();
	void update();
	void paintOn(HDC &hdc, Int2 position, Float2 scale = { 1.0f, 1.0f }) const;
};