#pragma once

#include <windows.h>

namespace Game {
	extern int scale;

	class Texture {
	public:
		virtual void paintOn(HDC &hdc, int left, int top, int width, int height, int mode = SRCCOPY) = 0;
	};

	class Bitmap : public Texture {
		HBITMAP const hbm;
		BITMAP bm;
		HDC buffer;
	public:
		Bitmap(LPCWSTR url) : hbm((HBITMAP)LoadImage(
			NULL, url, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE
		)) {
			GetObject(hbm, sizeof(BITMAP), &bm);
			buffer = CreateCompatibleDC(GetDC(NULL));
			SelectObject(buffer, hbm);
		}
		void paintOn(HDC &hdc, int left, int top, int width, int height, int mode = SRCCOPY) {
			StretchBlt(
				hdc, left, top, (int)(width * scale), (int)(height * scale),
				buffer, 0, 0, bm.bmWidth, bm.bmHeight,
				mode
			);
		}
	};

	class PureColor : public Texture {
	public:
		COLORREF color;
		PureColor(COLORREF color) : color(color) {}
		void paintOn(HDC &hdc, int left, int top, int width, int height, int mode = SRCCOPY) {
			SelectObject(hdc, CreatePen(PS_NULL, 0, 0));
			SelectObject(hdc, CreateSolidBrush(color));
			Rectangle(hdc, left, top, left + width, top + height);
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
			visible(false)
		{}
		void paintOn(HDC &hdc) const {
			texture->paintOn(hdc, x, y, width, height);
		}
	};
}