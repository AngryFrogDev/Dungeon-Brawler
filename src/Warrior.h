#ifndef __WARRIOR__
#define __WARRIOR__
#include "Character.h"

class Warrior : public Character {
public:
	Warrior(int x_pos, bool _fliped, int lane);
	~Warrior();

	void standingSpecial1();
	void standingSpecial2(const bool(&inputs)[MAX_INPUTS]);
	void crouchingSpecial1();
	void crouchingSpecial2();
	void jumpingSpecial1(const bool(&inputs)[MAX_INPUTS]);
	void jumpingSpecial2(const bool(&inputs)[MAX_INPUTS]);
	void doSuper();

	void updateAnimationOnBasicAttack(CHAR_ATT_TYPE type); // Just to use for super

private:

	int spin_speed;
	bool spin_object;

	int swordyuken_invencivility; // In milliseconds
	int swordyuken_jump_power;

	int projectile_speed;
	int projectile_duration; // in milliseconds
	int projectile_scale;

	int diveKickHeight;
	bool diveKicking;
	int jm_s1_angle;
	int jm_s2_angle;
	iPoint jm_s1_speed;
	iPoint jm_s2_speed;
	bool dive_kick_object;

	std::list<CHAR_ATT_TYPE> super_attack_list;
	CHAR_ATT_TYPE super_last_attack;
	std::list<CHAR_ATT_TYPE>::iterator super_iterator;
	int super_advance_speed;

};

#endif __WARRIOR__

