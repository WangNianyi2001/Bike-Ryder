#pragma once

#include <windows.h>
#include "config.h"

namespace Game {
	struct Int2 { int x, y; };

	class Texture {
		HBITMAP const hbm;
		BITMAP bm;
		HDC buffer;
		Int2 size;
	public:
		Texture(LPCWSTR url) :
			hbm((HBITMAP)LoadImage(
				NULL, url, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE
			)),
			buffer(CreateCompatibleDC(GetDC(NULL))),
			size({ bm.bmWidth, bm.bmHeight })
		{
			GetObject(hbm, sizeof(BITMAP), &bm);
			SelectObject(buffer, hbm);
		}
		void paintOn(HDC &hdc, Int2 position, Int2 dest_size, int mode = SRCCOPY) {
			StretchBlt(
				hdc,
				position.x * pixel_scale,
				position.y * pixel_scale,
				dest_size.x * pixel_scale,
				dest_size.y * pixel_scale,
				buffer, 0, 0, size.x, size.y,
				mode
			);
		}
	};

	struct Sprite {
		Int2 size, anchor, position;
		Int2 scale;
		int z;
		Texture *texture, *bg;
		bool visible;
		Sprite(
			Int2 size, Int2 anchor, Int2 position,
			Int2 scale, int z,
			Texture *texture, Texture *bg = nullptr
		) :
			size(size), anchor(anchor), position(position),
			scale(scale),
			z(z),
			texture(texture), bg(bg),
			visible(true)
		{}
		void paintOn(HDC &hdc) const {
			if(!visible)
				return;
			texture->paintOn(hdc, {
				position.x - scale.x * anchor.x,
				position.y - scale.y * anchor.y
			}, {
				scale.y * size.y,
				scale.y * size.y
			});
		}
	};
}