#pragma once
#pragma warning(disable:26495)

#include <windows.h>
#include "texture.h"

struct Frame {
	Int2 size, anchor;
	Texture *foreground, *mask;
	bool visible;
	Frame(
		Int2 size, Int2 anchor,
		Texture *foreground, Texture *mask = nullptr
	);
	void paintOn(HDC &hdc, Int2 position, Float2 scale = { 1.0f, 1.0f }) const;
};