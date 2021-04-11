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

Animation background({
	Frame{ 150, Texture(
		{ vwidth, vheight }, { 0, 0 },
		loadImage(L"background/0")
	) },
	Frame{ 150, Texture(
		{ vwidth, vheight }, { 0, 0 },
		loadImage(L"background/1")
	) },
	Frame{ 150, Texture(
		{ vwidth, vheight }, { 0, 0 },
		loadImage(L"background/2")
	) },
	Frame{ 150, Texture(
		{ vwidth, vheight }, { 0, 0 },
		loadImage(L"background/3")
	) },
	Frame{ 150, Texture(
		{ vwidth, vheight }, { 0, 0 },
		loadImage(L"background/4")
	) },
}, true);

Animation self_ride({
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
	) }
}, true);

Animation self_kick({
	Frame{ 150, Texture(
		{ 73, 95 }, { 42, 95 },
		loadImage(L"self/kick/0"),
		loadImage(L"self/kick/0.mask")
	) }
}, false);

Animation self_fall({
	Frame{ 50, Texture(
		{ 50, 94 }, { 9, 94 },
		loadImage(L"self/fall/0"),
		loadImage(L"self/fall/0.mask")
	) },
	Frame{ 50, Texture(
		{ 83, 67 }, { 6, 67 },
		loadImage(L"self/fall/1"),
		loadImage(L"self/fall/1.mask")
	) },
	Frame{ 50, Texture(
		{ 124, 39 }, { 0, 39 },
		loadImage(L"self/fall/2"),
		loadImage(L"self/fall/2.mask")
	) }
}, false);

void unloadImages() {
	for(Bitmap *image : images)
		delete image;
}
