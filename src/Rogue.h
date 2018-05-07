#pragma once
#include "Character.h"

struct character_deff;

class Rogue :
	public Character
{
public:
	Rogue(character_deff character, int x_pos, bool _fliped, int skin);

	void standingSpecial1(const bool(&inputs)[MAX_INPUTS]);

	void jumpingSpecial2(const bool(&inputs)[MAX_INPUTS]);


	~Rogue();


private:
	int projectile_speed = 7;
	int projectile_duration = 2000;

	int dash_speed = 10;
	bool grounded = true;
	bool has_airdash = true;

};

