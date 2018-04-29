#pragma once
#include "Character.h"

struct character_deff;

class Mage : public Character {
public:
	Mage(character_deff character, int x_pos, bool _fliped, int lane);
	~Mage();

	void standingSpecial1();
	void standingSpecial2(const bool(&inputs)[MAX_INPUTS]);
	void crouchingSpecial1();
	void crouchingSpecial2();
	void jumpingSpecial1(const bool(&inputs)[MAX_INPUTS]);
	void jumpingSpecial2(const bool(&inputs)[MAX_INPUTS]);
	bool standingSpecial1Condition();
	bool jumpingSpecial1Condition();
	bool jumpingSpecial2Condition();
	//void doSuper();

	void characterSpecificUpdates();

	//void giveItem(ITEMS type);
	//void takeAllItems();

private:
	int fireball_speed;
	int fireball_duration;
	iPoint fireball_emitter_offset;

	int jm_s1_angle;
	int jm_s1_max_height;
	iPoint jm_s1_backfire;
	iPoint air_fireball_speed;

	bool double_jump;
	iPoint double_jump_power;

	bool mine_placed;
	iPoint mine_position;
	Animation arcane_symbol;



};

