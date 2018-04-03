#ifndef __WARRIOR__
#define __WARRIOR__
#include "Character.h"

class Warrior : public Character {
public:
	Warrior(int x_pos, bool _fliped, int lane);
	~Warrior();

	int spin_speed;
	int projectile_speed;
	int projectile_duration; // in milliseconds
	int projectile_scale;

	void standingSpecial1();
	void standingSpecial2();

};

#endif __WARRIOR__

