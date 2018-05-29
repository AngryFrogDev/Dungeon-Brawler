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
	void deactivateSuperEmitters();


	~Rogue();


private:

	//XML
	//Projectiles
	int knife_speed;
	int knife_duration;
	int crossbow_recoil;
	int crossbow_angle;
	iPoint crossbow_speed;
	//ITEMS
	int item_damage_boost;
	int item_teleport_distance;
	//AIRDASH
	int dash_speed;
	int max_dash_frames;
	//ROLL
	int roll_speed;
	int max_roll_frames;
	//SLIDE
	float slide_speed;
	//SUPER
	int max_super_frames;


	//RUNTIME
	//ITEMS
	bool teleport_object;
	//Shit for airdash
	bool has_airdash;
	int current_dash_frames;
	//Shit for roll
	int current_roll_frames;
	//Super
	bool on_super = false;
	ParticleEmitter* super_emitter[3];
	ParticleEmitter* airdash_emitter = nullptr;
	int original_recoveries_array[12];
	int* recoveries_array[12];
	int current_super_frames;

	// I would leave it hardcoded for now
	std::list<CHAR_ATT_TYPE> rekka_attack_list;
	CHAR_ATT_TYPE rekka_last_attack;
	std::list<CHAR_ATT_TYPE>::iterator rekka_iterator;
	int rekka_advance_speed;
	int rekka_cancelability_window;
};

