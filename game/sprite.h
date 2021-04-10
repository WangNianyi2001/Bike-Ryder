#pragma once

#include <windows.h>

namespace Game {
	extern int scale;

	class Texture {
		HBITMAP const hbm;
		BITMAP bm;
		HDC buffer;
	public:
		Texture(LPCWSTR url) : hbm((HBITMAP)LoadImage(
			NULL, url, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE
		)) {
			GetObject(hbm, sizeof(BITMAP), &bm);
			buffer = CreateCompatibleDC(GetDC(NULL));
			SelectObject(buffer, hbm);
		}
		void paintOn(HDC &hdc, int left, int top, int width, int height, int mode = SRCCOPY) {
			StretchBlt(
				hdc, left * scale, top * scale, width * scale, height * scale,
				buffer, 0, 0, bm.bmWidth, bm.bmHeight,
				mode
			);
		}
	};

	struct Sprite {
		int x, y;
		int width, height;
		int z_index;
		Texture *texture, *bg;
		bool visible;
		Sprite(
			int x, int y,
			int width, int height,
			int z_index,
			Texture *texture, Texture *bg = nullptr
		) :
			x(x), y(y),
			width(width), height(height),
			z_index(z_index),
			texture(texture), bg(bg),
			visible(true)
		{}
		void paintOn(HDC &hdc) const {
			if(!visible)
				return;
			texture->paintOn(hdc, x, y, width, height);
		}
	};
}