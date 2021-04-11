#include "physics.h"
#include "config.h"
#include <algorithm>

std::vector<Physics *> Physics::all{};

void Physics::updateAll() {
	for(Physics *p : Physics::all)
		p->updatePhysics();
	sort(Physics::all.begin(), Physics::all.end());
}

Physics::Physics() {
	Physics::all.push_back(this);
}

Physics::~Physics() {
	Physics::all.erase(
		find(Physics::all.begin(), Physics::all.end(), this)
	);
}

bool Physics::operator>(Physics const &p) {
	return z > p.z;
}

void Physics::updatePhysics() {
	z += vz;
}

void Physics::render(HDC hdc) {
	float const scale = 1.0f / z;
	int const screen_y = (int)(horizon_y + scale * road_y_span);
	int const screen_x = (int)(scale * x + vwidth / 2.0f);
	paintOn(hdc, { screen_x, screen_y }, { scale * direction, scale });
}

void Character::updatePhysics() {
	Physics::updatePhysics();
	if(vz == 0)
		_ride->stop = true;
	else
		_ride->stop = false;
}

void Character::updateAnimation() {
	((Animation *)current->second)->update();
}

void Character::ride() {
	switchAppearance("ride");
	_ride->begin();
	active = true;
}

void Character::kick(int direction) {
	this->direction = direction;
	switchAppearance("kick");
	_kick->begin();
}

void Character::fall() {
	vz = 0;
	switchAppearance("fall");
	_fall->begin();
	active = false;
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

Character::~Character() {
	delete _ride;
	delete _kick;
	delete _fall;
}
