#pragma once

template<typename T> struct Pair { T x, y; };

using Int2 = Pair<int>;
using Float2 = Pair<float>;

class Texture {
public:
	Int2 size;
	HDC hdc;
	HBITMAP hbm;
	Texture(Int2 size);
	Texture(Texture const &reference);
	void paintOn(HDC &hdc, Int2 position, Int2 dest_size, int mode);
};

class PureColor : public Texture {
public:
	COLORREF color;
	PureColor(COLORREF color, Int2 size);
};

class Bitmap : public Texture {
public:
	Bitmap(LPCWSTR url);
};