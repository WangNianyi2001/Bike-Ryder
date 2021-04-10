#pragma once

#include "config.h"
#include "animation.h"
#include <set>

std::set<Bitmap *> images;
Bitmap *loadImage(wchar_t const *url) {
	wstring url_s = image_root;
	url_s.replace(url_s.find(L'$'), 1, url);
	Bitmap *res = new Bitmap(url_s.c_str());
	images.insert(res);
	return res;
}

Animation self_fall({
	Frame{ 150, Texture(
		{ 35, 94 }, { 19, 94 },
		loadImage(L"self/ride/0"),
		loadImage(L"self/ride/0.mask")
	) },
	Frame{ 75, Texture(
		{ 37, 95 }, { 19, 95 },
		loadImage(L"self/ride/1"),
		loadImage(L"self/ride/1.mask")
	) },
	Frame{ 150, Texture(
		{ 35, 94 }, { 16, 94 },
		loadImage(L"self/ride/2"),
		loadImage(L"self/ride/2.mask")
	) },
	Frame{ 75, Texture(
		{ 37, 95 }, { 19, 95 },
		loadImage(L"self/ride/3"),
		loadImage(L"self/ride/3.mask")
	) },
}, true);

Animation self_kick({
	Frame{ 150, Texture(
		{ 73, 95 }, { 42, 95 },
		loadImage(L"self/kick/0")
	) },
	}, true);

void unloadImages() {
	for(Bitmap *image : images)
		delete image;
}