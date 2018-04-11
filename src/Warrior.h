#ifndef __WARRIOR__
#define __WARRIOR__
#include "Character.h"

struct character_deff;

class Warrior : public Character {
public:
	Warrior(character_deff character,int x_pos, bool _fliped, int lane);
	~Warrior();

	void standingSpecial1();
	void standingSpecial2(const bool(&inputs)[MAX_INPUTS]);
	void crouchingSpecial1();
	void crouchingSpecial2();
	void jumpingSpecial1(const bool(&inputs)[MAX_INPUTS]);
	void jumpingSpecial2(const bool(&inputs)[MAX_INPUTS]);
	void doSuper();

	void giveItem(ITEMS type);
	void takeAllItems();

	void updateAnimationOnBasicAttack(CHAR_ATT_TYPE type); // Just to use for super

private:
	// -To load from xml-
	int spin_speed;

	int swordyuken_invencivility; // In milliseconds
	int swordyuken_jump_power;

	int projectile_speed;
	int projectile_duration; // in milliseconds
	int projectile_scale;

	int jm_s1_angle;
	int jm_s2_angle;
	iPoint jm_s1_speed;
	iPoint jm_s2_speed;

	item dive_kick_object, spin_object, swap_object; // Swap object is not being used
	// -To load from xml-

	// I would leave it hardcoded for now
	std::list<CHAR_ATT_TYPE> super_attack_list;
	CHAR_ATT_TYPE super_last_attack;
	std::list<CHAR_ATT_TYPE>::iterator super_iterator;
	int super_advance_speed;

};

#endif __WARRIOR__

