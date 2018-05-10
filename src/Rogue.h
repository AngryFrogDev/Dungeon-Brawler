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
	bool standingSpecial1Condition();


	void updateAnimationOnBasicAttack(CHAR_ATT_TYPE type); // Just to use for rekka
	void characterSpecificUpdates();
	void specificCharacterReset();

	void fillRecoveriesArray();
	void setAllRecoveriesTo(int value);
	void resetRecoveries();

	void addDamage();

	void giveItem(ITEMS type);


	~Rogue();


private:

	//XML
	//Projectiles
	int knife_speed = 7;
	int knife_duration = 1400;
	int crossbow_recoil = 10;
	int crossbow_angle = 30;
	iPoint crossbow_speed = { 30, 10 };
	//ITEMS
	int item_damage_boost = 3;
	int item_teleport_distance = 400;
	//AIRDASH
	int dash_speed = 20;
	int max_dash_frames = 5;
	//ROLL
	int roll_speed = 9;
	int max_roll_frames = 40;
	//SLIDE
	float slide_speed = 9;
	//SUPER
	int max_super_frames = 650;


	//RUNTIME
	//ITEMS
	bool teleport_object;
	//Shit for airdash
	bool has_airdash = true;
	int current_dash_frames = 0;
	//Shit for roll
	int current_roll_frames = 0;
	//Super
	bool on_super = false;
	ParticleEmitter* super_emitter = nullptr;
	ParticleEmitter* airdash_emitter = nullptr;
	int original_recoveries_array[12];
	int* recoveries_array[12];
	int current_super_frames = 0;

	// I would leave it hardcoded for now
	std::list<CHAR_ATT_TYPE> rekka_attack_list;
	CHAR_ATT_TYPE rekka_last_attack;
	std::list<CHAR_ATT_TYPE>::iterator rekka_iterator;
	int rekka_advance_speed;
	int rekka_cancelability_window;
};

