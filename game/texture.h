#pragma once
#pragma warning(disable:26495)

#include <windows.h>

template<typename T> struct Pair { T x, y; };

using Int2 = Pair<int>;
using Float2 = Pair<float>;

struct Layer {
	Int2 size;
	HDC hdc;
	HBITMAP hbm;
	Layer(Int2 size);
	Layer(Layer const &reference);
	void paintOn(HDC &hdc, Int2 position, Int2 dest_size, int mode);
};

struct PureColor : Layer {
	COLORREF color;
	PureColor(COLORREF color, Int2 size);
};

struct Bitmap : Layer {
public:
	Bitmap(LPCWSTR url);
};

struct Texture {
	Int2 size, anchor;
	Layer *foreground, *mask;
	bool visible;
	Texture(
		Int2 size, Int2 anchor,
		Layer *foreground, Layer *mask = nullptr
	);
	void paintOn(HDC &hdc, Int2 position, Float2 scale = { 1.0f, 1.0f }) const;
};