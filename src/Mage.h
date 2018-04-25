#pragma once
#include "Character.h"

struct character_deff;

class Mage : public Character {
public:
	Mage(character_deff character, int x_pos, bool _fliped, int lane);
	~Mage();
};

