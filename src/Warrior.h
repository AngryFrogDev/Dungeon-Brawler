#ifndef __WARRIOR__
#define __WARRIOR__
#include "Character.h"

class Warrior : public Character {
public:
	Warrior(int x_pos, bool _fliped, int lane);
	~Warrior();

	void standingSpecial1();
	void standingSpecial2();
	void crouchingSpecial1();
	void crouchingSpecial2();
	void jumpingSpecial1();
	void jumpingSpecial2();

private:

	int spin_speed;

	int swordyuken_invencivility; // In milliseconds

	int projectile_speed;
	int projectile_duration; // in milliseconds
	int projectile_scale;

	int diveKickHeight;
	bool diveKicking;
	int jm_s1_angle;
	int jm_s2_angle;
	iPoint jm_s1_speed;
	iPoint jm_s2_speed;

};

#endif __WARRIOR__

