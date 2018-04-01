#ifndef __WARRIOR__
#define __WARRIOR__
#include "Character.h"

class Warrior : public Character {
public:
	Warrior(int x_pos, bool _fliped);
	~Warrior();

	void standingSpecial2();
};

#endif __WARRIOR__

