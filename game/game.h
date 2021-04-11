#pragma once
#pragma warning(disable:28251)

#include "config.h"
#include "resources.h"
#include "text.h"
#include "physics.h"
#include <algorithm>

Character player(self_ride, self_kick, self_fall);
vector<Character *> NPCs;
void generateNPC() {
	Character *NPC = new Character(npc_ride, npc_kick, npc_fall);
	NPC->z = 5;
	NPC->vz = 0.2;
	NPC->x = rand() % 200 - 100;
	NPCs.push_back(NPC);
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
		if(player.x < -300)
			player.x = -300;
	}
	if(move_right) {
		player.x += 10.0f;
		if(player.x > 300)
			player.x = 300;
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
			NPC->fall();
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

void update() {
	// Physics
	move();
	Physics::updateAll();
	for(auto physics : Physics::all)
		physics->z -= player.vz;
	player.z = player_z;
	// Logic
	clipNPCs();
	if(crashed())
		player.fall();
	// Graphics
	player.updateAnimation();
	for(auto NPC : NPCs)
		NPC->updateAnimation();
	background.update();
}

void initGame() {
	background.begin();
	player.z = player_z;
}