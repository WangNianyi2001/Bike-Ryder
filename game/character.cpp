#include "character.h"
#include "config.h"

void Character::ride() {
	switchAppearance("ride");
	_ride->begin();
}

void Character::kick(int direction) {
	this->direction = direction;
	switchAppearance("kick");
	_kick->begin();
}

void Character::render(HDC hdc) {
	float const scale = 1.0f / z;
	int const screen_y = horizon_y + scale * road_y_span;
	int const screen_x = scale * x + vwidth / 2.0f;
	paintOn(hdc, { screen_x, screen_y }, { scale * direction, scale });
}

Character::Character(Animation const &ride, Animation const &kick, Animation const &fall) :
	_ride(new Animation(ride)),
	_kick(new Animation(kick)),
	_fall(new Animation(fall))
{
	appearances.insert(pair("ride", _ride));
	appearances.insert(pair("kick", _kick));
	appearances.insert(pair("fall", _fall));
	_kick->onEnd = [&](Animation *) {
		this->ride();
	};
	this->ride();
}
