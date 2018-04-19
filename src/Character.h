#ifndef __CHARACTER__
#define __CHARACTER__

#include "Animation.h"
#include "Application.h"
#include "mdInput.h"
#include "p2Point.h"
#include "mdRender.h"
#include "Timer.h"


#define MAX_INPUT_BUFFER 100

class Mix_Chunk;
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
	JM_S2,
	SUPER
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
	RECOVERY,

	STAND_BLOCKING, 
	CROUCH_BLOCKING,
	HIT,      // STAND_HIT/CROUCH_HIT ?
	JUGGLE,
	KNOCKDOWN,
	DEAD
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

enum BLOCK_TYPE {
	NO_BLOCK_TYPE = -1,
	MID,
	OVERHEAD,
	LOW
};
struct basic_attack_deff {
	int damage;
	int hitstun; //in miliseconds
	int blockstun; //in miliseconds
	int pushhit; //in pixels every second
	int pushblock; //in pixels
	SDL_Rect hitbox;
	iPoint pos_rel_char;
	int active_time; //in miliseconds
	CHAR_ATT_TYPE type;
	bool knockdown; //if the attack causes a knockdown or not
	iPoint juggle_speed;
	BLOCK_TYPE block_type;
	int recovery; //in milliseconds
	float animation_speed;
};

enum ITEMS {
	NO_ITEM = -1,
	SPECIAL_ITEM_1,
	SPECIAL_ITEM_2,
	SPECIAL_ITEM_3,

	STRENGTH_ITEM,
	INTELIGENCE_ITEM,
	DEXTERITY_ITEM,
	// There should be more items, but i don't remember them right now
};

struct item {
	ITEMS item_type;
	bool active;
};

class Player;

class Character {
public:
	Character();
	~Character();
						
	virtual void update(const bool (&inputs)[MAX_INPUTS]);	

	Character* oponent = nullptr;

	Player* partner = nullptr;

    // The first one is the collider belonging to this character
	void onCollision(collider* c1, collider* c2);

	void applyGravity();

	void setIfGrounded();

	void draw(SDL_Texture* graphic);

	bool manageSwap();

	void manageOponent();


	basic_attack_deff getAttackData(CHAR_ATT_TYPE attack_type) const;
	iPoint getPos() const;
	int getLateralLimit() const;
	CHAR_ATT_TYPE getAttackDoing() const;
	CHAR_STATE getCurrentState() const;
	void setFlip(bool flip);
	//void setProjectile(bool projectile);
	int getCurrentLife() const;
	int getMaxLife() const;
	int getCurrentSuperGauge() const;
	int getMaxSuperGauge() const;
	void resetCharacter();

	// Item management
	virtual void giveItem(ITEMS type) { return; }
	virtual void takeAllItems() { return; }
protected:	
	// Execute attack, rewritable for every type of character
	virtual void doAttack(const bool(&inputs)[MAX_INPUTS]);
	// Hitbox related functions
	void instanciateHitbox(CHAR_ATT_TYPE type);
	void deleteDeadHitboxes();
	collider* getCurrentAttackHitbox(); // Returns nullptr if no hitbox was found
	void deleteAttackHitbox(CHAR_ATT_TYPE type);
	void deleteAllMeleeHitboxes();

	// Swap related functions
	void manageGroundPosition();

	void updateAnimation(Animation& new_animation);

	void updateState(CHAR_STATE state, CHAR_ATT_TYPE attack = NO_ATT);
	void playCurrentSFX();
	void emmitCurrentParticle();
	void setCrouchingHurtbox(bool crouch);
	
	// Invencivility management
	void makeInvencibleFor(int invencible_time); // Variable in milliseconds
	void updateInvecibility();
	// Recovery management
	void askRecovery(int recovery);
	// Cancel management
	void manageCancel(const bool(&inputs)[MAX_INPUTS]);
	// Uses player's logic position, flip, offset and width to calculate the position to draw a collider
	int calculateDrawPosition(int offset, int size, bool x);
	//Special functions
	virtual void standingSpecial1() { return; };
	virtual void standingSpecial2(const bool(&inputs)[MAX_INPUTS]) { return; };
	virtual void crouchingSpecial1() { return; };
	virtual void crouchingSpecial2() { return; };
	virtual void jumpingSpecial1(const bool(&inputs)[MAX_INPUTS]) { return; };
	virtual void jumpingSpecial2(const bool(&inputs)[MAX_INPUTS]) { return; };
	virtual bool standingSpecial1Condition() {return true; }
	virtual bool jumpingSpecial1Condition() {return true; }
	virtual bool jumpingSpecial2Condition() {return true; }

	virtual void doSuper() { return; }
	// Input buffer functions
	bool lookInBuffer(CHARACTER_INPUTS input, int window);
	void fillBuffer(const bool(&inputs)[MAX_INPUTS]);
	void pushIntoBuffer(CHARACTER_INPUTS input);
	bool checkForSuper(int window);

protected:

	// Variables to assign from xml
	CHAR_TYPE type;

	float scale;

	int crouching_hurtbox_offset;
	iPoint standing_hurtbox_size;

	Animation idle, walk_forward, walk_back, crouch, light_attack, heavy_attack, jump, crouching_light, crouching_heavy, jumping_light, jumping_heavy, standing_special1, standing_special2, jumping_special1, jumping_special2, crouching_special1, crouching_special2, standing_hit, standing_block, crouching_block, knockdown, dead, taunt;

	basic_attack_deff st_l, st_h, cr_l, cr_h, jm_l, jm_h, st_s1, st_s2, cr_s1, cr_s2, jm_s1, jm_s2, super;


	iPoint jump_power;
	float gravity;
	int max_life;
	int max_super_gauge;
	int super_gauge_gain_hit;
	int super_gauge_gain_block;
	int super_gauge_gain_strike;

	int lateral_limit;  

	int walk_speed;

	int crouch_particle_offset;

	// In miliseconds
	int invencibility_on_wakeup;

	int super_window;
	int cancelability_window;

	//If the projectile has already been thrown, no other projectile should be
	//bool projectile; // PROVISIONAL: This should only belong to warrior

	int shadow_offset;
	SDL_Rect shadow_rect;


	// Sound effects
	Mix_Chunk* s_jump;
	Mix_Chunk* s_light_sword_block;
	Mix_Chunk* s_heavy_sword_block;
	Mix_Chunk* s_light_sword_whiff;
	Mix_Chunk* s_heavy_sword_whiff;
	Mix_Chunk* s_light_sword_impact;
	Mix_Chunk* s_heavy_sword_impact;
	Mix_Chunk* s_standing_special_1; 
	Mix_Chunk* s_standing_special_2;
	Mix_Chunk* s_jumping_special_1; 
	Mix_Chunk* s_crouching_special_1;
	Mix_Chunk* s_crouching_special_2; 
	Mix_Chunk* s_man_death;
	Mix_Chunk* s_super;

	// Variables to load from constructor
	iPoint starting_position;
	int bottom_lane;
	int upper_lane;
	Player* owner;

	// Variables to modify in runtime
	int ground_position;
	iPoint logic_position;
	iPoint draw_position;
	iPoint draw_size;
	
	iPoint velocity;

	bool crouching_hurtbox;

	int current_life;		
	int current_super_gauge;

	bool grounded;

	bool fliped;

	bool death;

	//If the hitbox of the attack has been already instanciated, it should,'t be instanciated again
	bool instanciated_hitbox; 
	bool state_first_tick;
	bool state_second_tick;

	bool hit;
	//Maybe current_stun and moment_hit should be a timer instead
	int moment_hit; 


	// Entity collider
	collider* hurtbox = nullptr;	
	collider* pushbox = nullptr;
	std::list<collider*> hitboxes; //It should be a list, as a character can have multiple active hitboxes


	CHAR_STATE current_state;
	CHAR_ATT_TYPE attack_doing;
	basic_attack_deff attack_recieving;


	Animation* current_animation = nullptr;

	// Time to stop invencibility
	int stop_invencibility;
	Timer invencible_timer;

	int current_recovery; // In milliseconds
	Timer recovery_timer;
	CHARACTER_INPUTS input_buffer[MAX_INPUT_BUFFER];
public:
	//Swap variables
	int lane; // 1 = bottom  2 = top This is important
	bool readyToSwap = false;
	bool swapRequested = false;
	bool swapDone = false;

protected:
	pugi::xml_document config_doc;
	pugi::xml_node config;
	pugi::xml_node data;
};

#endif //__CHARACTER__
