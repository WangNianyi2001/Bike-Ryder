#include "animation.h"

Animation::Animation(initializer_list<Frame> frames, bool loop, void (*onEnd)(Animation *)) :
	loop(loop), onEnd(onEnd)
{
	this->frames.insert(this->frames.end(), frames);
}

void Animation::begin() {
	last_frame = GetTickCount64();
	active = frames.begin();
	stop = false;
}

void Animation::update() {
	if(stop)
		return;
	ULONGLONG elapsed = GetTickCount64() - last_frame;
	while(active != frames.end()) {
		if(active->interval > elapsed)
			return;
		elapsed -= active->interval;
		last_frame += active->interval;
		++active;
	}
	stop = true;
	if(onEnd)
		onEnd(this);
	if(loop)
		begin();
}

void Animation::paintOn(HDC &hdc, Int2 position, Float2 scale) const {
	if(active == frames.end()) {
		frames.begin()->texture.paintOn(hdc, position, scale);
		return;
	}
	active->texture.paintOn(hdc, position, scale);
}