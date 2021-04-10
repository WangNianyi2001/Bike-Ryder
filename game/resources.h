#pragma once

#include "animation.h"

Animation fall({
	Frame{ 150, Texture(
		{ 35, 94 }, { 19, 94 },
		new Bitmap(L"../images/self/ride/0.bmp"),
		new Bitmap(L"../images/self/ride/0.mask.bmp")
	) },
	Frame{ 75, Texture(
		{ 37, 95 }, { 19, 95 },
		new Bitmap(L"../images/self/ride/1.bmp"),
		new Bitmap(L"../images/self/ride/1.mask.bmp")
	) },
	Frame{ 150, Texture(
		{ 35, 94 }, { 16, 94 },
		new Bitmap(L"../images/self/ride/2.bmp"),
		new Bitmap(L"../images/self/ride/2.mask.bmp")
	) },
	Frame{ 75, Texture(
		{ 37, 95 }, { 19, 95 },
		new Bitmap(L"../images/self/ride/3.bmp"),
		new Bitmap(L"../images/self/ride/3.mask.bmp")
	) },
}, true);