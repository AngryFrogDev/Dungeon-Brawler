#pragma once
#include "Character.h"

struct character_deff;

class Rogue :
	public Character
{
public:
	Rogue(character_deff character, int x_pos, bool _fliped, int skin);
	~Rogue();

};

