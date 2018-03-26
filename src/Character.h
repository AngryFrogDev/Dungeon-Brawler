#ifndef __CHARACTER__
#define __CHARACTER__

#include "Animation.h"
#include "Application.h"
#include "mdInput.h"
#include "p2Point.h"
#include "mdRender.h"

enum CHAR_TYPE {
	DEF_CHAR,
	WARRIOR,
	MAGE,
	ROGUE,
	PALADIN
};

enum CHAR_ATT_TYPE {
	NO_ATT,
	CR_L,
	CR_H,
	CR_S1,
	CR_S2,
	
	ST_L,
	ST_H,
	ST_S1,
	ST_S2,

	JM_L,
	JM_H,
	JM_S1,
	JM_S2
};
enum CHAR_STATE {
	NULL_STATE,
	IDLE,
	WALKING_BACK,
	WALKING_FORWARD,
	CROUCHING,
	JUMPING,
	ATTACKING,
	SWAPPING,

	BLOCKING,
	HIT,
	JUGGLE,
	KNOCKDOWN
};

enum CHARACTER_INPUTS {
	NULL_INPUT = -1,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	LIGHT_ATTACK,
	HEAVY_ATTACK,
	SPECIAL_1,
	SPECIAL_2,
	GRAB,
	SWITCH,
	MAX_INPUTS
};
struct basic_attack_deff {
	int damage;
	int hitstun; //in miliseconds
	int blockstun; //in miliseconds
	int pushhit; //in pixels
	int pushblock; //in pixels
	SDL_Rect hitbox;
	iPoint pos_rel_char;
	int active_time; //in miliseconds
	CHAR_ATT_TYPE type;
};

class Player;

class Character {
public:
	Character();
	~Character();
						
	virtual void update(const bool (&inputs)[MAX_INPUTS]);		

	void onCollision(collider* c1, collider* c2); // The first one is the collider belonging to this character

	void applyGravity();

	void setIfGrounded();

	void draw(SDL_Texture* graphic) const;

	//Execute attack, rewritable for every type of character
	virtual void doAttack();		
	void instanciateHitbox(CHAR_ATT_TYPE type);
	basic_attack_deff getCurrentAttackData();

protected:
	void updateAnimation(Animation& new_animation);

protected:
	CHAR_TYPE type;

	iPoint position;
	iPoint velocity;

	int current_life; 								
	int max_life;	
	int walk_speed;	

	bool grounded;
	
	bool fliped;

	bool hit = false;
	int moment_hit; //Maybe current_stun and moment_hit should be a timer instead

	// Entity collider
	collider* hurtbox;	
	collider* hitbox; //It should be a list, as a character can have multiple active hitboxes

	CHAR_STATE current_state;
	CHAR_ATT_TYPE attack_doing;
	basic_attack_deff attack_recieving;

	Animation* current_animation;
	Animation idle, walk_forward, walk_back, crouch, light_attack, heavy_attack, jump, crouching_light, crouching_heavy, jumping_light, jumping_heavy, standing_hit;


	basic_attack_deff st_l, st_h, cr_l, cr_h, jm_l, jm_h;
	bool instanciated_hitbox;
	Player* owner;


	//PROVISIONAL should be read from xml
	iPoint jump_power;
	float gravity = 1;
	int bottom_lane = 300;


public:
	int lane = 1; //Provisional 1 = bottom  2 = top

};

#endif //__CHARACTER__
