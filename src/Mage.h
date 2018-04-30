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
	void doSuper();

	void characterSpecificUpdates();

	//void giveItem(ITEMS type);
	//void takeAllItems();

private:
	int fireball_speed;
	int fireball_duration;
	iPoint fireball_emitter_offset;


	int air_fireball_angle;
	int air_fireball_max_height;
	iPoint air_fireball_backfire;
	iPoint air_fireball_speed;

	bool double_jump;
	iPoint double_jump_power;

	bool mine_placed;
	iPoint mine_position;
	Animation arcane_symbol;

	int meteorits_life;
	int first_meteorit_height;
	iPoint meteorits_offset;
	int meteorits;
	int meteorits_rows;
	int meteorits_rows_offset;
	iPoint meteorits_speed;
	bool meteorits_spawned;







};

