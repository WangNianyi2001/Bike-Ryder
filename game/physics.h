#pragma once

#include "sprite.h"
#include "animation.h"
#include <set>

struct Physics : Sprite {
	static std::vector<Physics *> all;
	static void sort();
	static void updateAll();
	int direction = 1;
	float z, x, vz;
	Physics();
	Physics(Scalable *appearance);
	~Physics();
	virtual void updatePhysics();
	void render(HDC hdc);
};

struct Character : Physics {
	bool active = true;
	Animation *_ride, *_kick, *_fall;
	void updatePhysics();
	void updateAnimation();
	void ride();
	void kick(int direction);
	void fall(int direction);
	Character(Animation const &ride, Animation const &kick, Animation const &fall);
	~Character();
};