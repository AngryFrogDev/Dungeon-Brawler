#pragma once
#include "Character.h"

struct character_deff;

class ParticleEmitter;

class Rogue :
	public Character
{
public:
	Rogue(character_deff character, int x_pos, bool _fliped, int skin);

	void standingSpecial1(const bool(&inputs)[MAX_INPUTS]);
	void crouchingSpecial1();
	void jumpingSpecial1(const bool(&inputs)[MAX_INPUTS]);
	void jumpingSpecial2(const bool(&inputs)[MAX_INPUTS]);

	void characterSpecificUpdates();

	~Rogue();


private:
	int projectile_speed = 7;
	int projectile_duration = 2000;

	//Shit for airdash
	int dash_speed = 20;
	bool has_airdash = true;
	int max_dash_frames = 5;
	int current_dash_frames = 0;

	//Shit for roll
	int roll_speed = 7;
	int max_roll_frames = 40;
	int current_roll_frames = 0;

	int crossbow_recoil = 10;
	int crossbow_angle = 30;
	iPoint crossbow_speed = { 30, 10 };

	ParticleEmitter* airdash_emitter = nullptr;
};

