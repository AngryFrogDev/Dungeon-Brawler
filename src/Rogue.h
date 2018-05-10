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
	void standingSpecial2(const bool(&inputs)[MAX_INPUTS]);
	void crouchingSpecial1();
	void crouchingSpecial2();
	void jumpingSpecial1(const bool(&inputs)[MAX_INPUTS]);
	void jumpingSpecial2(const bool(&inputs)[MAX_INPUTS]);
	void doSuper();
	bool jumpingSpecial2Condition();


	void updateAnimationOnBasicAttack(CHAR_ATT_TYPE type); // Just to use for rekka
	void characterSpecificUpdates();

	void fillRecoveriesArray();
	void setAllRecoveriesTo(int value);
	void resetRecoveries();

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
	int roll_speed = 9;
	int max_roll_frames = 40;
	int current_roll_frames = 0;

	int crossbow_recoil = 10;
	int crossbow_angle = 30;
	iPoint crossbow_speed = { 30, 10 };

	float slide_speed = 9;

	bool on_super = false;
	ParticleEmitter* super_emitter = nullptr;
	ParticleEmitter* airdash_emitter = nullptr;
	int original_recoveries_array[12];
	int* recoveries_array[12];
	int max_super_frames = 800;
	int current_super_frames = 0;

	// I would leave it hardcoded for now
	std::list<CHAR_ATT_TYPE> rekka_attack_list;
	CHAR_ATT_TYPE rekka_last_attack;
	std::list<CHAR_ATT_TYPE>::iterator rekka_iterator;
	int rekka_advance_speed;
	int rekka_cancelability_window;
};

