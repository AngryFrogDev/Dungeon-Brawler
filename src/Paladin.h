#pragma once
#include "Character.h"
struct character_deff;
class ParticleEmitter;

class Paladin : public Character {
public:
	Paladin(character_deff character, int x_pos, bool _fliped, int skin);
	~Paladin();

	void standingSpecial1(const bool(&inputs)[MAX_INPUTS]);
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

	void giveItem(ITEMS type);
	void takeAllItems();

private:
	Animation parry_reaction;
	bool parry_reacting;
	int parry_start;
	bool air_hammer_thrown;
	bool just_teleported;

	bool teleport_object;

	// To load from XML
	int parry_reaction_invencivility;
	int parry_healing;

	int parry_duration;
	int miss_parry_recovery;

	int st_s2_speed;
	int st_s2_invencivility;

	int hammer_duration;
	iPoint hammer_speed;

	iPoint slam_speed;
	int slam_max_height;

	int air_hammer_speed;
	int air_hammer_duration;

	int super_healing;

	int endurance_object_plus;

	ParticleEmitter* healing_emitter = nullptr;
};
