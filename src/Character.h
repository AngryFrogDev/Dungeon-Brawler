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
	DEF_CHAR = -1,
	WARRIOR,
	MAGE,
	ROGUE,
	PALADIN,
	CHAR_TOTAL
};

// Sound effects
enum CHAR_SOUNDS {
	S_JUMP = 0,
    S_LIGHT_SWORD_BLOCK,
    S_HEAVY_SWORD_BLOCK,
	S_LIGHT_SWORD_WHIFF,
    S_HEAVY_SWORD_WHIFF,
    S_LIGHT_SWORD_IMPACT,
    S_HEAVY_SWORD_IMPACT,
    S_STANDING_SPECIAL_1,
    S_STANDING_SPECIAL_2,
    S_JUMPING_SPECIAL_1,
    S_CROUCHING_SPECIAL_1,
    S_CROUCHING_SPECIAL_2,
    S_DEATH,
    S_SUPER,
	MAX_SOUNDS
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
	STOPPED, // Does idle but can't move nor attack
	RECOVERY,
	TAUNT,

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
	START,
	MAX_INPUTS
};

enum BLOCK_TYPE {
	NO_BLOCK_TYPE = -1,
	MID,
	OVERHEAD,
	LOW
};
struct basic_attack_deff {
	basic_attack_deff() {
		damage = 0;
		hitstun = 0;
		blockstun = 0;
		pushhit = 0;
		pushblock = 0;
		hitbox = { 0,0,0,0 };
		pos_rel_char = { 0,0 };
		active_time = 0;
		type = NO_ATT;
		knockdown = false;
		juggle_speed = { 0,0 };
		block_type = NO_BLOCK_TYPE;
		recovery = 0;
		animation_speed = 0;

	}
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
	int frame_delay;
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

class Player;
struct character_deff;

class Character {
public:
	Character(character_deff character, int x_pos, int _fliped, int skin);
	virtual ~Character();
						
	virtual void update(const bool (&inputs)[MAX_INPUTS]);	

    // The first one is the collider belonging to this character
	void onCollision(collider* c1, collider* c2);

	void applyGravity();

	void setIfGrounded();

	void draw(SDL_Texture* graphic);


	basic_attack_deff getAttackData(CHAR_ATT_TYPE attack_type) const;
	iPoint getPos() const;
	int getLateralLimit() const;
	CHAR_ATT_TYPE getAttackDoing() const;
	CHAR_STATE getCurrentState() const;
	void setFlip(bool flip);
	int getCurrentLife() const;
	int getMaxLife() const;
	int getCurrentSuperGauge() const;
	int getMaxSuperGauge() const;
	virtual ITEMS getItem() const { return NO_ITEM; };
	bool notAllowFlip();
	CHAR_TYPE getType() const;
	void resetCharacter();
	virtual void specificCharacterReset() {return;}

	// Item management
	virtual void giveItem(ITEMS type) { return; }
	virtual void takeAllItems() { return; }

	// Taunt management
	void tauntFor(int _taunt_duration);
	// Animation pause
	void setAnimationPause(bool active);
	// State update externally
	void setState(CHAR_STATE state); // Just use it to set to stopped or idle, please
protected:	
	// Execute attack, rewritable for every type of character
	virtual void doAttack(const bool(&inputs)[MAX_INPUTS]);
	// Hitbox related functions
	void instanciateHitbox(basic_attack_deff type);
	void deleteDeadHitboxes();
	collider* getCurrentAttackHitbox(); // Returns nullptr if no hitbox was found
	collider* getAttackHitbox(CHAR_ATT_TYPE type);
	void deleteAttackHitbox(CHAR_ATT_TYPE type, collider* hitbox = nullptr);
	void deleteAllMeleeHitboxes();
	void deleteAllHitboxes();

	void updateAnimation(Animation& new_animation);


	void updateState(CHAR_STATE state, CHAR_ATT_TYPE attack = NO_ATT);
	void playCurrentSFX();
	void emmitCurrentParticle();
	void setCrouchingHurtbox(bool crouch);
	
	// Hurtbox size management
	void hurtboxSizeManagement();
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
	virtual void standingSpecial1(const bool(&inputs)[MAX_INPUTS]) { return; };
	virtual void standingSpecial2(const bool(&inputs)[MAX_INPUTS]) { return; };
	virtual void crouchingSpecial1() { return; };
	virtual void crouchingSpecial2() { return; };
	virtual void jumpingSpecial1(const bool(&inputs)[MAX_INPUTS]) { return; };
	virtual void jumpingSpecial2(const bool(&inputs)[MAX_INPUTS]) { return; };
	virtual bool standingSpecial1Condition() {return true; }
	virtual bool jumpingSpecial1Condition() {return true; }
	virtual bool jumpingSpecial2Condition() {return true; }

	virtual void characterSpecificUpdates() {return; }

	virtual void doSuper() { return; }
	//Juggle limit
	bool juggleLimit(CHAR_ATT_TYPE type);
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

	Animation idle, walk_forward, walk_back, crouch, light_attack, heavy_attack, jump, crouching_light, crouching_heavy, jumping_light, jumping_heavy, standing_special1, standing_special2, jumping_special1, jumping_special2, crouching_special1, crouching_special2, standing_hit, standing_block, crouching_block, knockdown, dead, super_anim, taunt;

	basic_attack_deff st_l, st_h, cr_l, cr_h, jm_l, jm_h, st_s1, st_s2, cr_s1, cr_s2, jm_s1, jm_s2, super;

	std::list<CHAR_ATT_TYPE> non_flip_attacks;
	std::list<CHAR_ATT_TYPE> crouching_hurtbox_attacks;



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

	int taunt_start;
	int taunt_duration;
	int normal_taunt_duration;

	// In miliseconds
	int invencibility_on_wakeup;

	int super_window;
	int cancelability_window;

	int shadow_offset;
	SDL_Rect shadow_rect;


	// Variables to load from constructor
	iPoint starting_position;
	Player* owner;

	// Variables to modify in runtime
	int ground_position;
	fPoint logic_position;
	iPoint draw_position;
	iPoint draw_size;
	
	fPoint velocity;

	bool crouching_hurtbox;

	int current_life;		
	int current_super_gauge;

	bool grounded;

	bool fliped;

	bool death;

	std::list<CHAR_ATT_TYPE> juggle_attacks_recieved;

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
	//int lane; // 1 = bottom  2 = top This is important
	int skin_id; // 0 = normal, 1 = recolor 1, 2 = recolor 2...

protected:
	// Sound effects
	Mix_Chunk * s_jump;
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
	Mix_Chunk* s_death;
	Mix_Chunk* s_super;

};

#endif //__CHARACTER__
