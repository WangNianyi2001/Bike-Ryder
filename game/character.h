#pragma once

#include "sprite.h"
#include "animation.h"

class Character : public Sprite {
public:
	int direction = 1;
	float x, z, vx, vz;
	Animation *_ride, *_kick, *_fall;
	void updatePhysics();
	void ride();
	void kick(int direction);
	void fall();
	void render(HDC hdc);
	Character(Animation const &ride, Animation const &kick, Animation const &fall);
};