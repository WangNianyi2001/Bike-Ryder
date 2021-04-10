#pragma once

#include "game.h"
#include <vector>
#include <random>

using namespace Game;
using namespace std;

Scene level;
void levelInit();

constexpr int max_z = 100;
constexpr int horizon_y = 200;
constexpr int zyk = horizon_y / max_z;

class WorldObject {
protected:
	float x, y, z;
	int width, height;
public:
	Sprite sprite;
	WorldObject(int width, int height, float x, float y, float z) :
		width(width), height(height),
		x(x), y(y), z(z),
		sprite(Sprite(0, 0, width, height, (int)z, nullptr))
	{}
	void updatePosition() {
		sprite.z_index = (int)z;
		if(z > max_z) {
			sprite.visible = false;
			return;
		}
		sprite.visible = true;
		sprite.width = (int)(width / z);
		sprite.height = (int)(height / z);
		sprite.x = x / z + vwidth / 2;
		sprite.y = vheight - (y / z + zyk * z);
	}
};

class Character : public WorldObject {
public:
	static int width, height;
	Character(float x, float y = 0, float z = 1) : WorldObject(
		Character::width, Character::height,
		x, y, z
	) {
		sprite.texture = new PureColor(RGB(
			rand() & 255,
			rand() & 255,
			rand() & 255
		));
		updatePosition();
	}
};
int Character::width = 50;
int Character::height = 100;

vector<Character *> characters;
Character *self;

void enter() {
}

LRESULT paint(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	level.paintOn(hdc);
	EndPaint(hWnd, &ps);
	return 0;
}

void levelInit() {
	level.onEnter = &enter;
	self = new Character(0.0f, 100.0f);
	level.addSprite(&self->sprite);
	level.addHandler(WM_PAINT, &paint);
}