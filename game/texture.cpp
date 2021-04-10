#include <windows.h>
#include "texture.h"

Texture::Texture(Int2 size) :
	hdc(CreateCompatibleDC(GetDC(NULL))),
	size(size) {
}

Texture::Texture(Texture const &reference) : Texture(reference.size) {
	hbm = CreateBitmap(size.x, size.y, 1, 32, nullptr);
	SelectObject(hdc, hbm);
	BitBlt(hdc, 0, 0, size.x, size.y, reference.hdc, 0, 0, SRCCOPY);
}

void Texture::paintOn(HDC &dest, Int2 position, Int2 dest_size, int mode) {
	StretchBlt(
		dest,
		position.x,
		position.y,
		dest_size.x,
		dest_size.y,
		hdc, 0, 0, size.x, size.y,
		mode
	);
}

PureColor::PureColor(COLORREF color, Int2 size) : Texture(size), color(color) {
	hbm = CreateBitmap(size.x, size.y, 1, 32, nullptr);
	SelectObject(hdc, hbm);
	SelectObject(hdc, GetStockObject(NULL_PEN));
	SelectObject(hdc, CreateSolidBrush(color));
	Rectangle(hdc, 0, 0, size.x + 1, size.y + 1);
}

Bitmap::Bitmap(LPCWSTR url) : Texture({ 0, 0 }) {
	hbm = (HBITMAP)LoadImage(
		NULL, url, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE
	);
	BITMAP bm;
	GetObject(hbm, sizeof(BITMAP), &bm);
	size = { bm.bmWidth, bm.bmHeight };
	SelectObject(hdc, hbm);
}