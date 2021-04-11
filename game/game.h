#pragma once
#pragma warning(disable:28251)

#include "config.h"
#include "resources.h"
// #include "text.h"
#include "physics.h"
#include <algorithm>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

void playSound(LPCWSTR url) {
	PlaySound(url, NULL, SND_ASYNC | SND_FILENAME);
}

Character player(self_ride, self_kick, self_fall);
vector<Character *> NPCs;
void generateNPC() {
	Character *NPC = new Character(npc_ride, npc_kick, npc_fall);
	NPC->z = generate_z;
	NPC->vz = avg_npc_speed + ((float)rand() / RAND_MAX - .5) * npc_speed_tolerance;
	NPC->x = rand() % runway_width - runway_width / 2;
	NPCs.push_back(NPC);
	Physics::sort();
}

void generateBuilding() {
	int const index = rand() % buildings.size();
	Physics *building = new Physics(&buildings[index]);
	building->z = generate_z;
	building->vz = 0;
	building->x = -runway_width / 2;
	if(rand() & 1) {
		building->x *= -1;
		building->direction = -1;
	}
	Physics::sort();
}

bool inKickRange(Character *NPC, int direction) {
	float const
		delta_x = direction * (NPC->x - player.x),
		delta_z = abs(NPC->z - player_z);
	return
		NPC->active &&
		delta_z <= kick_z_range &&
		delta_x >= 0.0 && delta_x <= kick_x_range;
}

bool crashedWith(Character *NPC) {
	float const delta_z = NPC->z - player_z;
	return
		NPC->active &&
		abs(NPC->x - player.x) <= crash_x_tolerance &&
		delta_z <= crash_z_tolerance && delta_z >= 0;
}

bool crashed() { return any_of(NPCs.begin(), NPCs.end(), crashedWith); }

bool accelerate = false, decelerate = false, move_left = false, move_right = false;

map<WPARAM, bool *> move_state_table{
	{ 'W', &accelerate },
	{ 'S', &decelerate },
	{ 'A', &move_left },
	{ 'D', &move_right }
};
void updateMoveState(WPARAM key, bool state) {
	if(!move_state_table.count(key))
		return;
	*move_state_table[key] = state;
}
void move() {
	if(!player.active)
		return;
	player.vz *= speed_decay;
	if(accelerate) {
		player.vz += 0.01f;
		if(player.vz > 1)
			player.vz = 1;
	}
	if(decelerate) {
		player.vz -= 0.01f;
		if(player.vz < 0)
			player.vz = 0;
	}
	if(move_left) {
		player.x -= 10.0f;
		if(player.x < -runway_width / 2)
			player.x = -runway_width / 2;
	}
	if(move_right) {
		player.x += 10.0f;
		if(player.x > runway_width / 2)
			player.x = runway_width / 2;
	}
}

map<WPARAM, int> kick_table{
	{ 'Q', -1 },
	{ 'E', 1 }
};
void kick(WPARAM key) {
	if(!player.active)
		return;
	if(!kick_table.count(key))
		return;
	int direction = kick_table[key];
	player.kick(direction);
	for(auto NPC : NPCs) {
		if(inKickRange(NPC, direction))
			NPC->fall(direction);
	}
}

vector<Character *>::iterator removeNPC(vector<Character *>::iterator it) {
	delete *it;
	return NPCs.erase(it);
}

bool toBeClipped(Physics *p) {
	return p->z <= 0.0f || p->z >= far_clip_z;
}

void clipNPCs() {
	for(auto it = NPCs.begin(); it != NPCs.end(); ) {
		if(toBeClipped(*it))
			it = removeNPC(it);
		else
			++it;
	}
}

void clipPhysics() {
	for(auto p : Physics::all) {
		if(toBeClipped(p))
			delete p;
	}
}

void update() {
	// Physics
	move();
	Physics::updateAll();
	for(auto physics : Physics::all)
		physics->z -= player.vz;
	player.z = player_z;
	// Logic
	clipNPCs();
	clipPhysics();
	if(crashed())
		player.fall(player.direction);
	// Graphics
	player.updateAnimation();
	for(auto NPC : NPCs)
		NPC->updateAnimation();
	background.update();
	if(player.vz)
		runway.update();
}

void initGame() {
	background.begin();
	runway.begin();
	player.z = player_z;
	playSound(bgm);
}