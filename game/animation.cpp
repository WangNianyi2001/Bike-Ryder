#include "animation.h"

Frame::Frame(
	Int2 size, Int2 anchor,
	Texture *foreground, Texture *mask
) :
	size(size), anchor(anchor),
	foreground(foreground), mask(mask),
	visible(true)
{
	if(mask) {
		mask->paintOn(foreground->hdc, { 0, 0 }, { size.x, size.y }, DSTINVERT);
		mask->paintOn(foreground->hdc, { 0, 0 }, { size.x, size.y }, SRCAND);
	}
}

void Frame::paintOn(HDC &hdc, Int2 position, Float2 scale) const {
	if(!visible)
		return;
	if(mask) {
		foreground->paintOn(
			hdc, {
				(int)(position.x - scale.x * anchor.x),
				(int)(position.y - scale.y * anchor.y)
			}, {
				(int)(scale.x * size.x),
				(int)(scale.y * size.y)
			}, DSTINVERT
		);
	}
	foreground->paintOn(
		hdc, {
			(int)(position.x - scale.x * anchor.x),
			(int)(position.y - scale.y * anchor.y)
		}, {
			(int)(scale.y * size.y),
			(int)(scale.y * size.y)
		}, NOTSRCERASE
	);
}

class Sprite {
public:
};