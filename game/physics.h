#pragma once

#include "sprite.h"
#include "animation.h"
#include <set>

struct Physics : Sprite {
	static std::vector<Physics *> all;
	static void updateAll();
	int direction = 1;
	float z, x, vz;
	Physics();
	~Physics();
	virtual void updatePhysics();
	void render(HDC hdc);
	bool operator>(Physics const &p);
};

struct Character : Physics {
	bool active = true;
	Animation *_ride, *_kick, *_fall;
	void updatePhysics();
	void updateAnimation();
	void ride();
	void kick(int direction);
	void fall();
	Character(Animation const &ride, Animation const &kick, Animation const &fall);
	~Character();
};